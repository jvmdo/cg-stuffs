//http://www.youtube.com/user/thecplusplusguy
//FIRST OPENGL 3.2+ PROGRAM :D
//post-processing
#include <iostream>
#include <SDL/SDL.h>
#include "matrices.h"
#include "GLee.h"
#include "shader.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "camera.h"
#include <vector>
#include <string>
#include <fstream>
#include "sceneLoader.h"

camera cam;
matrices pipeline;
meshLoader* scene;
shader* mainShader;
shader* quadRenderShader;
shader* skyboxShader;
shader* reflectShader;

unsigned int cubeMap;
//data->vertex->fragment

SDL_Surface* loadTexture(std::string& filename)
{
	unsigned int num;
	glGenTextures(1,&num);
	SDL_Surface* img=IMG_Load(filename.c_str());
	if(img==NULL)
	{
		std::cout << "img was not loaded" << std::endl;
		return 0;
	}
	SDL_PixelFormat form={NULL,32,4,0,0,0,0,0,0,0,0,0xff000000,0x00ff0000,0x0000ff00,0x000000ff,0,255};
	SDL_Surface* img2=SDL_ConvertSurface(img,&form,SDL_SWSURFACE);
	SDL_FreeSurface(img);
	return img2;
}


unsigned int loadCubemap(std::string* filenames)//6 filename
{
	//right,left,top,bottom,near,far
	unsigned int tex;
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	
	for(int i=0;i<6;i++)
	{
		SDL_Surface* img=loadTexture(filenames[i]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGBA,img->w,img->h,0,GL_RGBA,GL_UNSIGNED_INT_8_8_8_8,img->pixels);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		SDL_FreeSurface(img);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	return tex;
}


unsigned int createTexture(int w,int h,bool isDepth=false)
{
	unsigned int textureId;
	glGenTextures(1,&textureId);
	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexImage2D(GL_TEXTURE_2D,0,(!isDepth ? GL_RGBA8 : GL_DEPTH_COMPONENT),w,h,0,isDepth ? GL_DEPTH_COMPONENT : GL_RGBA,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	
	int i;
	i=glGetError();
	if(i!=0)
	{
		std::cout << "Error happened while loading the texture: " << i << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D,0);
	return textureId;
}

unsigned int FBO;
unsigned int renderTexture,depthTexture;

mesh* quad;
mesh* cube;
void init()
{
	glClearColor(0,0,0.5,1);
	
	pipeline.perspective(50,640.0/480.0,1,1000);
	
	glEnable(GL_DEPTH_TEST);
	mainShader=new shader("vertex.vs","fragment.frag");
	quadRenderShader=new shader("quadRender.vs","quadRender.frag");
	skyboxShader=new shader("skyboxShader.vs","skyboxShader.frag");
	reflectShader=new shader("reflection.vs","reflection.frag");
	scene=new meshLoader("test.blend");
	
	renderTexture=createTexture(640,480);
	depthTexture=createTexture(640,480,true);
	glGenFramebuffers(1,&FBO);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);
	//GL_COLOR_ATTACHMENT0
	//GL_DEPTH_ATTACHMENT
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,renderTexture,0);

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthTexture,0);


	int i=glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(i!=GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not OK, status=" << i << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
	//create the quad here
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		//1.
		tmp.position.change(-1.0,1.0,0.0);
		tmp.U=0;
		tmp.V=1;
		vertices.push_back(tmp);
		//2.
		tmp.position.change(-1.0,-1.0,0.0);
		tmp.U=0;
		tmp.V=0;
		vertices.push_back(tmp);
		//3.
		tmp.position.change(1.0,-1.0,0.0);
		tmp.U=1;
		tmp.V=0;
		vertices.push_back(tmp);
		//4.
		tmp.position.change(1.0,1.0,0.0);
		tmp.U=1;
		tmp.V=1;
		vertices.push_back(tmp);
		
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);		
		
		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);
		quad=new mesh(&vertices,&indices);
	}
	
	{
		std::vector<unsigned int> indices;
		std::vector<vertexData> vertices;
		vertexData tmp;
		tmp.position.change(-1.0,1.0,1.0);
		tmp.color.change(1.0,0.0,0.0);
		vertices.push_back(tmp);
		
		tmp.position.change(-1.0,-1.0,1.0);
		tmp.color.change(0.0,1.0,0.0);
		vertices.push_back(tmp);
		
		tmp.position.change(1.0,-1.0,1.0);
		tmp.color.change(0.0,0.0,1.0);
		vertices.push_back(tmp);
		
		tmp.position.change(1.0,1.0,1.0);
		tmp.color.change(1.0,1.0,0.0);
		vertices.push_back(tmp);
		
		tmp.position.change(-1.0,1.0,-1.0);
		tmp.color.change(1.0,0.0,1.0);
		vertices.push_back(tmp);
		
		tmp.position.change(-1.0,-1.0,-1.0);
		tmp.color.change(1.0,0.0,0.0);
		vertices.push_back(tmp);
		
		tmp.position.change(1.0,-1.0,-1.0);
		tmp.color.change(1.0,1.0,1.0);
		vertices.push_back(tmp);
		
		tmp.position.change(1.0,1.0,-1.0);
		tmp.color.change(0.0,1.0,1.0);
		vertices.push_back(tmp);
		
		
		//front face
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);
		
		
		//left face
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(6);
		
		indices.push_back(3);
		indices.push_back(7);
		indices.push_back(6);
		
		//back face
		indices.push_back(4);
		indices.push_back(5);
		indices.push_back(6);
		
		indices.push_back(4);
		indices.push_back(7);
		indices.push_back(6);
		
		//right face
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(5);
		
		indices.push_back(0);
		indices.push_back(4);
		indices.push_back(5);
		
		
		//top face
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(4);
		
		indices.push_back(3);
		indices.push_back(4);
		indices.push_back(7);
		
		
		//bottom face
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(6);
		
		indices.push_back(1);
		indices.push_back(5);
		indices.push_back(6);
		cube=new mesh(&vertices,&indices);
	}
	
	std::string filename[6];
	filename[0]="skybox/right.bmp";
	filename[1]="skybox/left.bmp";
	filename[2]="skybox/top.bmp";
	filename[3]="skybox/bottom.bmp";
	filename[4]="skybox/front.bmp";
	filename[5]="skybox/back.bmp";
	cubeMap=loadCubemap(filename);
	
	
}
float angle=0.0;
void display()
{
	//rendering to texture...
	pipeline.perspective(50,640.0/480.0,0.5,1000);
	glDisable(GL_DEPTH_TEST);
	pipeline.loadIdentity();
	
	cam.Control(pipeline);
	
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	pipeline.matrixMode(MODEL_MATRIX);
	skyboxShader->useShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMap);
		glUniform1i(glGetUniformLocation(skyboxShader->getProgramId(),"cubeMap"),0);
		pipeline.updateMatrices(skyboxShader->getProgramId());
		cube->draw(skyboxShader->getProgramId());
	skyboxShader->delShader();	
	
	cam.UpdateCamera(pipeline);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	pipeline.matrixMode(MODEL_MATRIX);
//	glUniform3f(glGetUniformLocation(reflectShader->getProgramId(),"lightPos"),0,1,2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP,cubeMap);
	
	reflectShader->useShader();
		glUniform3f(glGetUniformLocation(reflectShader->getProgramId(),"cameraPosition"),cam.getLocation().x,cam.getLocation().y,cam.getLocation().z);
		glUniform1i(glGetUniformLocation(reflectShader->getProgramId(),"cubeMap"),0);
		pipeline.updateMatrices(reflectShader->getProgramId());
		scene->draw(reflectShader->getProgramId());
	reflectShader->delShader();	
	
}

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(640,480,32,SDL_OPENGL);
	Uint32 start;
	SDL_Event event;
	bool running=true;
	init();
	bool b=false;
	while(running)
	{
		start=SDL_GetTicks();
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running=false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running=false;
							break;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					cam.mouseIn(true);
					break;
					
			}
		}
		display();
		SDL_GL_SwapBuffers();
		if(1000.0/30>SDL_GetTicks()-start)
			SDL_Delay(1000.0/30-(SDL_GetTicks()-start));
	}
	delete scene;
	delete mainShader;
	delete quadRenderShader;
	delete cube;
	delete skyboxShader;
	delete reflectShader;
	delete quad;
	SDL_Quit();
}
