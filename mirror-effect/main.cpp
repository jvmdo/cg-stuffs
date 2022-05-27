#include <windows.h>
#include "GL/glut.h"
#include <stdlib.h>
#include <stdio.h>

#define TIMER 33

#ifdef WIN32
#pragma warning( disable : 4305)
#endif


static float xrot;
GLUquadric *quad;


void drawScene (GLenum order)
{
    GLfloat pos[4] = {-2.8, 5., 1.8, 1.};

    glLightfv (GL_LIGHT1, GL_POSITION, pos);

	glPushMatrix();

	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glFrontFace (order);

	/* Draw the walls */
    glColor3f (1., 1., 1.);
	glBegin(GL_QUADS);
	  glNormal3f (1., 0., 0.);
	    glVertex3f (-3., 3., 4.);
	    glVertex3f (-3., -3., 4.);
	    glVertex3f (-3., -3., -3.);
	    glVertex3f (-3., 3., -3.);
	  glNormal3f (0., 0., 1.);
		glVertex3f (-3., 3., -3.);
		glVertex3f (-3., -3., -3.);
		glVertex3f (3., -3., -3.);
		glVertex3f (3., 3., -3.);
	  glNormal3f (-1., 0., 0.);
		glVertex3f (3., 3., -3.);
		glVertex3f (3., -3., -3.);
		glVertex3f (3., -3., 3.);
		glVertex3f (3., 3., 3.);
	glEnd();
	glDisable (GL_CULL_FACE);

	/* Draw the cylinder */
    glRotatef (xrot, 1., 0., 0.);
    glTranslatef (0., 0., -1.);

    glColor3f (.5, .5, 1.);
    glPushMatrix ();
    glTranslatef (0., 0., 2.);
    gluDisk (quad, 0., .25, 18, 1);
    glPopMatrix ();

    gluCylinder (quad, .25, .25, 2., 18, 8);

    glPushMatrix ();
    glScalef (1., 1., -1.);
    gluDisk (quad, 0., .25, 18, 1);
    glPopMatrix ();

	glPopMatrix();
}


static void display( void )
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity ();
    gluLookAt (0., 1., 7.,
               -1., 0., 0.,
               0., 1., 0.);
	glRotatef (-xrot*.35, 0., 1., 0.);

	/* Draw reflected scene first */

	glPushMatrix ();
	/* Mirrors lies in YZ plane, so scale by -1.0 in X axis */
	glScalef (-1., 1., 1.);
	/* Mirror is 2.0 units from origin, so translate by 4.0 */
	glTranslatef (4., 0., 0.);
    drawScene(GL_CW);
	glPopMatrix ();

	/* draw mirror */
	glClear (GL_DEPTH_BUFFER_BIT);
	glPushAttrib (0xffffffff);
	glDisable (GL_LIGHTING);
	/* Create imperfect reflector effect by blending a black
	   mirror over the reflected scene with alpha of 0.05 */
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f (0., 0., 0., 0.05);
	glBegin (GL_QUADS);
	glVertex3f (-2., 1., 3.);
	glVertex3f (-2., -1., 3.);
	glVertex3f (-2., -1., -2.5);
	glVertex3f (-2., 1., -2.5);
	glEnd ();
	glPopAttrib();

	/* Draw the real scene */
    drawScene(GL_CCW);

    glutSwapBuffers();

	{
		int err = glGetError();
		if (err != GL_NO_ERROR)
			printf ("Error %x\n", err);
	}
}

void reshape(int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective (50., (float)w/(float)h, 1., 20.);
}

static void cbMainMenu (int value)
{
    if (value == 99) {
        exit (0);
    }
}


static void timer (int value)
{
    xrot += 1.f;
    if (xrot > 360.f) xrot -= 360.f;

    glutPostRedisplay ();
    glutTimerFunc (TIMER, timer, 0);
}


static void init ()
{
    int mainMenu;

    xrot = 0.;

    glDisable (GL_DITHER);
    glEnable (GL_DEPTH_TEST);

    {
        GLfloat white[4] = {1., 1., 1., 1.};
        GLfloat black[4] = {0., 0., 0., 0.};

        /* Set up light1 */
        glEnable (GL_LIGHTING);
        glEnable (GL_LIGHT1);
        glLightfv (GL_LIGHT1, GL_DIFFUSE, white);
        glLightfv (GL_LIGHT1, GL_SPECULAR, white);

        /* ambient and diffuse will track glColor */
        glEnable (GL_COLOR_MATERIAL);
        glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
        glMaterialfv (GL_FRONT, GL_SPECULAR, white);
		glMaterialf (GL_FRONT, GL_SHININESS, 20.);
    }


	quad = gluNewQuadric ();

    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glutTimerFunc (TIMER, timer, 0);

    mainMenu = glutCreateMenu (cbMainMenu);
    glutAddMenuEntry ("Quit", 99);
    glutAttachMenu (GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv)
{
    glutInit (&argc,argv);
    glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (300,300);
    glutInitWindowPosition (0,0);
    glutCreateWindow ("Two pass mirror");

    init ();

    glutMainLoop ();
}
