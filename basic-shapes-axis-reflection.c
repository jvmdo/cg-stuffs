#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535897932384626433832795
#define C 2
#define T 3
#define S 4

float crm[][C] = {{1, 0}, {0, 1}}; // circle reflection matrix
float trm[][T] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // triangle reflection matrix
float srm[][S] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}; // square reflection matrix

char shape;

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y)
{
	switch (key) {
		case 27: //Escape key
			exit(0);
			break;

        case 'l':
            drawScene();
            break;

        case 's':
            drawSquareScene();
            shape = 's';
            break;

        case 't':
            drawTriangleScene();
            shape = 't';
            break;

        case 'c':
            drawCircleScene();
            shape = 'c';
            break;

        case '1':
            crm[0][0] = 1; crm[0][1] = 0;
            crm[1][0] = 0; crm[1][1] = -1;

            trm[0][0] = -1; trm[0][1] = 0; trm[0][2] = 0;
            trm[1][0] = 0; trm[1][1] = 1; trm[1][2] = 0;
            trm[2][0] = 0; trm[2][1] = 0; trm[2][2] = 1;

            srm[0][0] = -1; srm[0][1] = 0; srm[0][2] = 0; srm[0][3] = 0;
            srm[1][0] = 0; srm[1][1] = 1; srm[1][2] = 0; srm[1][3] = 0;
            srm[2][0] = 0; srm[2][1] = 0; srm[2][2] = 1; srm[2][3] = 0;
            srm[3][0] = 0; srm[3][1] = 0; srm[3][2] = 0; srm[3][3] = 1;

            handleKeypress(shape, 0, 0);
            break;

        case '2':
            crm[0][0] = 1; crm[0][1] = 0;
            crm[1][0] = 0; crm[1][1] = 1;

            trm[0][0] = 1; trm[0][1] = 0; trm[0][2] = 0;
            trm[1][0] = 0; trm[1][1] = 1; trm[1][2] = 0;
            trm[2][0] = 0; trm[2][1] = 0; trm[2][2] = 1;

            srm[0][0] = 1; srm[0][1] = 0; srm[0][2] = 0; srm[0][3] = 0;
            srm[1][0] = 0; srm[1][1] = 1; srm[1][2] = 0; srm[1][3] = 0;
            srm[2][0] = 0; srm[2][1] = 0; srm[2][2] = 1; srm[2][3] = 0;
            srm[3][0] = 0; srm[3][1] = 0; srm[3][2] = 0; srm[3][3] = 1;

            handleKeypress(shape, 0, 0);
            break;

        case '3':
            crm[0][0] = -1; crm[0][1] = 0;
            crm[1][0] = 0; crm[1][1] = 1;

            trm[0][0] = 1; trm[0][1] = 0; trm[0][2] = 0;
            trm[1][0] = 0; trm[1][1] = -1; trm[1][2] = 0;
            trm[2][0] = 0; trm[2][1] = 0; trm[2][2] = 1;

            srm[0][0] = 1; srm[0][1] = 0; srm[0][2] = 0; srm[0][3] = 0;
            srm[1][0] = 0; srm[1][1] = -1; srm[1][2] = 0; srm[1][3] = 0;
            srm[2][0] = 0; srm[2][1] = 0; srm[2][2] = -1; srm[2][3] = 0;
            srm[3][0] = 0; srm[3][1] = 0; srm[3][2] = 0; srm[3][3] = 1;

            handleKeypress(shape, 0, 0);
            break;

        case '4':
            crm[0][0] = -1; crm[0][1] = 0;
            crm[1][0] = 0; crm[1][1] = -1;

            trm[0][0] = -1; trm[0][1] = 0; trm[0][2] = 0;
            trm[1][0] = 0; trm[1][1] = -1; trm[1][2] = 0;
            trm[2][0] = 0; trm[2][1] = 0; trm[2][2] = 1;

            srm[0][0] = -1; srm[0][1] = 0; srm[0][2] = 0; srm[0][3] = 0;
            srm[1][0] = 0; srm[1][1] = -1; srm[1][2] = 0; srm[1][3] = 0;
            srm[2][0] = 0; srm[2][1] = 0; srm[2][2] = 1; srm[2][3] = 0;
            srm[3][0] = 0; srm[3][1] = 0; srm[3][2] = 0; srm[3][3] = 1;

            handleKeypress(shape, 0, 0);
            break;
	}
}

//Initializes 3D rendering
void initRendering()
{
	//Makes 3D drawing work when something is in front of something else
	glEnable(GL_DEPTH_TEST);
}

//Called when the window is resized
void handleResize(int w, int h)
{
	//Tell OpenGL how to convert from coordinates to pixel values
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
	//Set the camera perspective
	glLoadIdentity(); //Reset the camera
	gluPerspective(45.0,                  //The camera angle
				   (double)w / (double)h, //The width-to-height ratio
				   1.0,                   //The near z clipping coordinate
				   200.0);                //The far z clipping coordinate
}

// This function multiplies
// mat1[][] and mat2[][], and
// stores the result in res[][]
void multiplyMatrices(int ord, float mat1[][ord], float mat2[][ord], float res[][ord])
{
    int i, j, k;
    for (i = 0; i < ord; i++){
        for (j = 0; j < ord; j++){
            res[i][j] = 0;
            for (k = 0; k < ord; k++)
                res[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}

void triangle(int r)
{
    float x1, x2, x3;
    float y1, y2, y3;

    x1 = -0.5f; x2 = -1.0f; x3 = -1.5f;
    y1 =  0.5f; y2 =  1.5f; y3 =  0.5f;

    float tm[][3] = {{x1, y1, 1}, {x2, y2, 1}, {x3, y3, 1}}; // triangle matrix

//    if( r == 0 ){
//        glBegin(GL_TRIANGLES); //Begin triangle coordinates
//            glVertex3f(tm[0][0], tm[0][1], -5.0f);
//            glVertex3f(tm[1][0], tm[1][1], -5.0f);
//            glVertex3f(tm[2][0], tm[2][1], -5.0f);
//        glEnd(); //End triangle coordinates
//    }

	if( r == 1 ){
        float res[T][T];
        multiplyMatrices(T, tm, trm, res);
        for( int i = 0; i < T; i++ ){
            for( int j = 0; j < T; j++ ){
                printf("%f ", res[i][j]);
            }
            printf("\n");
        }

        glBegin(GL_TRIANGLES); //Begin triangle coordinates
            glVertex3f(res[0][0], res[0][1], -5.0f);
            glVertex3f(res[1][0], res[1][1], -5.0f);
            glVertex3f(res[2][0], res[2][1], -5.0f);
        glEnd(); //End triangle coordinates
	}
}

void square(int r)
{
    float x1, x2, x3, x4;
    float y1, y2, y3, y4;

    x1 = -1.5f; x2 =  -0.5f; x3 =  -0.5f, x4 = -1.5f;
    y1 =  1.5f; y2 =   1.5f; y3 =   0.5f, y4 =  0.5f;

    float sm[][S] = {{x1, y1, 1, 1}, {x2, y2, 1, 1}, {x3, y3, 1, 1}, {x1, y4, 1, 1}}; // square matrix

//    if( r == 0 ){
//        glBegin(GL_QUADS); //Begin quadrilateral coordinates
//            glVertex3f(sm[0][0], sm[0][1], -5.0f);
//            glVertex3f(sm[1][0], sm[1][1], -5.0f);
//            glVertex3f(sm[2][0], sm[2][1], -5.0f);
//            glVertex3f(sm[3][0], sm[3][1], -5.0f);
//        glEnd(); //End quadrilateral coordinates
//    }

    if( r == 1 ){
        float res[S][S];
        multiplyMatrices(S, sm, srm, res);
        for( int i = 0; i < S; i++ ){
            for( int j = 0; j < S; j++ ){
                printf("%f ", res[i][j]);
            }
            printf("\n");
        }

        glBegin(GL_QUADS); //Begin quadrilateral coordinates
            glVertex3f(res[0][0], res[0][1], -5.0f);
            glVertex3f(res[1][0], res[1][1], -5.0f);
            glVertex3f(res[2][0], res[2][1], -5.0f);
            glVertex3f(res[3][0], res[3][1], -5.0f);
        glEnd(); //End quadrilateral coordinates
	}
}

void circle(int r) {
    //Specifies the radius of the circle
    static float radius = 0.7;
    float cm[C][C] = {{0, 0}, {0, 0}};

    //if( r == 0 ){
        //glBegin(GL_POLYGON); //Begin draw Circle
            for(float i = 0; i < 2 * PI; i += PI / 48){ //<-- Change this Value i += 0.065 < 6.283
                glVertex3f(cos(i)*radius-1, sin(i)*radius+1, -5.0f);
                if( (int) i == 3 ){
                    cm[0][0] = cos(i)*radius;
                    cm[0][1] = sin(i)*radius + PI/(48*C);
                }
                if( (int) i == 6 ){
                    cm[1][0] = cos(i)*radius;
                    cm[1][1] = sin(i)*radius + PI/(48*C);
                }
            }
        //glEnd(); //End draw Circle
    //}

    if( r == 1 ){
        float res[C][C]; // change to T T otherwise
        multiplyMatrices(C, cm, crm, res);
        for( int i = 0; i < C; i++ ){
            for( int j = 0; j < C; j++ ){
                printf("%f ", res[i][j]);
            }
            printf("\n");
        }

        glBegin(GL_POLYGON); //Begin draw Circle
		for(float i = 0; i < 2 * PI; i += PI / 48){ //<-- Change this Value i += 0.065 < 6.283
			glVertex3f(cos(i)*radius + (((res[0][1]+res[1][1])*C) - (PI/48)),
                       sin(i)*radius + (((res[0][0]+res[1][0])*C*C) + (PI/(48*C))),
                       -5.0f);
		}
	glEnd(); //End draw Circle
    }
}

void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutSwapBuffers();
}

//Draws the 3D scene
void drawTriangleScene()
{
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

    triangle( 1 );

	glutSwapBuffers(); //Send the 3D scene to the screen
}

void drawSquareScene()
{
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

    square( 1 );

	glutSwapBuffers(); //Send the 3D scene to the screen
}

void drawCircleScene()
{
	//Clear information from last draw
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

    circle( 1 );

	glutSwapBuffers(); //Send the 3D scene to the screen
}

int main(int argc, char** argv)
{
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600); //Set the window size

	//Create the window
	glutCreateWindow("Basic Shapes Reflection");
	initRendering(); //Initialize rendering

	//Set handler functions for drawing, keypresses, and window resizes
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMainLoop(); //Start the main loop

	return 0;
}
