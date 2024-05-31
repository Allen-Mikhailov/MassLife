#include <stdio.h>
#include <glew.h>
#include <glut.h>

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow("GLEW Test");
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		
	}

	glutMainLoop();
	return 0;
}