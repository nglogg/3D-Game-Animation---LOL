



#include <gl/glew.h>
#include "ms3d.h"
#include "obj.h"
#include <gl/freeglut.h>


GLfloat zoominout = -1000;
GLfloat totatex = 25;
GLfloat totatey = 45;
GLfloat totatez = 0;
GLint indexcurrent =0;



CMs3d command1;
CMs3d command2;
CMs3d command3;
CMs3d command4;
CMs3d command5;
CMs3d command0;
CMs3d command6;



void init(void)
{
	GLint buf, sbuf;

	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glEnable(GL_DEPTH_TEST);
	glGetIntegerv(GL_SAMPLE_BUFFERS, &buf);
	glGetIntegerv(GL_SAMPLES, &sbuf );
	
	command0.Load("roshan.ms3d");
	command1.Load("thresh2run.ms3d");
	command2.Load("nnuu.ms3d");
 	command3.Load("popy3.ms3d");
 	command4.Load("popy2run.ms3d");
	command5.Load("zileannoel.ms3d");
	command6.Load("gragasrun.ms3d");
	


}
void display(void)
{



	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glTranslatef(  0.0, 0.0 ,zoominout );
	glRotatef(totatex, 1.0, 0.0, 0.0 );
	glRotatef(totatey, 0.0, 1.0, 0.0 );
	glRotatef(totatez, 0.0, 0.0, 1.0 );
	glColor3f(1.0, 1.0, 1.0 );
	glPointSize(10);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	
	
	
// 	glTranslatef( -400,0,-200);
// 	
 	glTranslatef( 0,0,-250);
 	command0.Render(false,0, 1);
	glTranslatef( 0, 0, 300 );
	command1.Render(true, 1,8);
//	glLoadIdentity();
	glRotatef(-40.0, 0.0, 1.0, 0.0 );
	glTranslatef( 0,0,-600);
	command2.Render(true, 2, 4);
	glTranslatef( 0,0,600);
	glRotatef(-40.0, 0.0, 1.0, 0.0 );
	glRotatef(-40.0, 0.0, 1.0, 0.0 );
	glTranslatef(150,50,-200);
	command5.Render(true, 5, 8);
	glTranslatef(-150,-50,-200);
	glTranslatef( 0,0,600);
	glRotatef(40.0, 0.0, 1.0, 0.0 );
	
	glRotatef(145, 0.0, 1.0, 0.0 );
	glTranslatef(0.0,0.0,-100);
	command6.Render(true,6,8);
	glRotatef(45,0.0,1.0,0.0);
	glTranslatef( -50, 0, -500 );
 	command3.Render(true, 3,4);
 	glTranslatef( 350, 0, 0 );
	glRotatef(-30, 0.0, 1.0, 0.0);
 	command4.Render(true,4,6);

	



	glDisable(GL_MULTISAMPLE);

	glutSwapBuffers();



}
void reshape(int w, int h )
{
	glViewport( 0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 60.0, (GLfloat) w/ (GLfloat) h, 0.01, 10000.0 );
	glMatrixMode(GL_MODELVIEW);
	// 	glLoadIdentity();
	// 	gluLookAt( 0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
}

void keyboard( unsigned char key, int x, int y )
{
	switch(key)
	{
	case 'd':
		zoominout += 50;
		glutPostRedisplay();
		break;
	case 'f':
		zoominout -=50;
		glutPostRedisplay();
		break;
	case 's':
		totatex +=5;
		if(totatex >= 360 )
			totatex =0;
		glutPostRedisplay();
		break;
	case 'z':
		totatez +=5;
		if(totatez >= 360 )
			totatez =0;
		glutPostRedisplay();
		break;
	case 'c':
		totatey +=5;
		if(totatey >= 360 )
			totatey =0;
		glutPostRedisplay();
		break;
	
		
	default:
		break;
	}
}



void processTimer(int value )
{


	glutTimerFunc( 0.1, processTimer, 1 );
	glutPostRedisplay();

}

int main(int argc, char** argv )
{
	glutInit(&argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE | GLUT_DEPTH );
	glutInitWindowSize( 1280, 768 );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc( 0.1, processTimer, 1 );
	glutMainLoop();
	return 1;
}