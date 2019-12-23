#include <gl/glut.h>

#include "application.h"

Application* getApplication();
Application* app;

void display(void) {
	app->display();
}

//	creates the window with the initial size
void createWindow(const char* title, int h, int w) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(w, h);
	glutCreateWindow(title);
}

void timerFunc(int v) {
	app->update();
	float time_interval = app->getTimeInterval();
	glutTimerFunc(time_interval, timerFunc, 1);
}

//	when the window gets resized this will get called to handle the aspect ratio
void resize(int w, int h) {
	app->resize(w, h);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	float time_interval = 10;
	app = getApplication();
	app->setTimeInterval(time_interval);
	createWindow("Demo", app->getHeight(), app->getWidth());
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutTimerFunc(time_interval, timerFunc, 1);
	app->initGraphics(0.0f, 0.0f, 0.0f);
	glutMainLoop();	// loops endlessly to refresh the window
	delete app;
	return 0;
}