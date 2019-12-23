#include <gl/glut.h>
#include "application.h"
#include "coreMath.h"
#include "contactResolver.h"
#include "Demo.h"
#include <stdio.h>
#include <cassert>
#include <iostream>
const Vector2 Vector2::GRAVITY = Vector2(0, -9.81);	// cosntant 
const int NO_OF_CIRCLES = 12;	

Demo::Demo() {

	//default resolution of the window
	w = 600, h = 600;

	n_range = 100.0;

	// initalises the simulator with the width, height and n_range (initially 100) of the window
	sim = new Simulator(w, h, n_range);
		



	 //-- VELOCITY SIMULATION --

	createCircle(10, 30, { -80.0, 0 }, { 0, 50 }, 0.4, 30.0f);
	createCircle(10, 30, { 80.0, 0.0 }, { -60, 0 }, 1.0, 30.0f);
	createCircle(10, 30, { 0.0, -80.0 }, { 0, 60 }, 1.0, 30.0f);
	createCircle(10, 30, { 0.0, 80.0 }, { 0, -60 }, 1.0, 30.0f);

	createCircle(10, 30, { 80, 84 }, { 0, -108 }, 1.0, 30.0f);
	createCircle(10, 30, { -80.0, -80.0 }, { 0, 108 }, 1.0, 30.0f);
	createCircle(10, 30, { 80.0, -80.0 }, { -108, 0 }, 1.0, 30.0f);
	createCircle(10, 30, { -80.0, 80.0 }, { 108, 0 }, 1.0, 30.0f);

	createCircle(10, 30, { 50.0, 0.0 }, { 0, -108 }, 1.0, 30.0f);
	createCircle(10, 30, { -50.0, 0.0 }, { 0, 108 }, 1.0, 30.0f);
	createCircle(10, 30, { 0.0, 50.0 }, { 108, 0 }, 1.0, 30.0f);
	createCircle(10, 30, { 0.0, -50.0 }, { -108, 0 }, 1.0, 30.0f);

}

void Demo::createStillCircle(int r, int ss, Vector2 pos) {
	Circle* circle = new Circle(r, ss, ss);
	circle->setPosition(pos);
	circle->setVelocity({ 0, 0 });
	circle->setDamping(1.0);
	circle->setInverseMass(0);
	circle->clearAccumulator();
	sim->addShape(circle);
}

void Demo::createCircle(int r, int ss, Vector2 pos, Vector2 vel, double d, float m) {
	//when called a new circle will be created from the circle class (which inherits from shape), providing the radius and stacks/slices for the circle
	Circle* circle = new Circle(r, ss, ss);

	// a vector2 object will then be used to set the position and velocity for the circle
	circle->setPosition(pos);
	circle->setVelocity(vel);

	// then set the other physics attributes for the circle including damping, mass and gravity
	circle->setDamping(d);
	circle->setAcceleration(Vector2::GRAVITY * 20.0f);
	circle->setMass(m);
	circle->clearAccumulator();

	//the shape will then get add to the vector of shapes (in the simulator class)
	sim->addShape(circle);
}

void Demo::display() {

	Application::display();

	// loops for the amount of circles which will be displayed and draws them onto the window
	for (int circle = 0; circle < NO_OF_CIRCLES; circle++) {
		drawCircle(circle, 255, 255, 255);
	}

	glutSwapBuffers();
}

void Demo::drawCircle(int shape_pos, int r, int g, int b) {
	// draws the shape onto the window
	const Vector2& pos = sim->getShapes()[shape_pos]->getPosition();
	glPushMatrix();

	// setting its position to translateand the attributes including radius, slicesand stacks
	glTranslatef(pos.x, pos.y, 0);
	glColor3ub(r, g, b);
	glutSolidSphere(sim->getShapes()[shape_pos]->getRadius() , sim->getShapes()[shape_pos]->getSlices(), sim->getShapes()[shape_pos]->getStacks());
	glPopMatrix();
}

void Demo::update() {
	// indicates the time between each update
	float dur = time_interval / 1000;
	float duration = 10.0f / 1000;
	int g = 10;

	//check the window bounds to see if a shape is colliding
	for (int x = 0; x < sim->getShapes().size(); x++) {
		sim->checkBounds(x, sim->getShapes()[x]->getRadius(), dur, g, w, h);
		w = Application::w;
		h = Application::h;
	}
	// runs the physics simulation for each of the shapes in the simulators vector
	sim->runPhysics(dur, w, h);
	Application::update();
}

Application* getApplication() {
	return new Demo();
}

Demo::~Demo() {
	delete sim;
}