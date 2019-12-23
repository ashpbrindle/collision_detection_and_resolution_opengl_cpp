#include <gl/glut.h>
#include "application.h"

void Application::initGraphics(float r, float g, float b) {
	glClearColor(r, g, b, 1.0f);
}

void Application::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Application::update() {
	glutPostRedisplay();
}

void Application::resize(int w, int h) {
	GLfloat aspect_ratio = (GLfloat)w / (GLfloat)h;
	if (h == 0) { h = 1; }
	
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
	{
		Application::w = n_range;
		Application::h = n_range / aspect_ratio;
		glOrtho(-n_range, n_range, -n_range / aspect_ratio, n_range / aspect_ratio, -n_range * 2.0f, n_range * 2.0f);
	}
	else
	{
		Application::w = n_range * aspect_ratio;
		Application::h = n_range;
		glOrtho(-n_range * aspect_ratio, n_range * aspect_ratio, -n_range, n_range, -n_range * 2.0f, n_range * 2.0f);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}