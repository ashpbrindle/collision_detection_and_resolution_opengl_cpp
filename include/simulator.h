#include <vector>
#include "contactResolver.h"
#pragma once
/* The simulator will manage all of the shapes in the 
window as well as calling the relevent classes to resolve 
collisions */

class Simulator {
public:
	CollisionManager manager;	// manages the collision
	typedef std::vector<Shape*> Shapes;	// all of the shapes in the system
	Simulator(float w, float h, float n_range);	// initially creates the quad tree
	~Simulator();
	void addShape(Shape* sh);	// adds the shape to the vector
	std::vector<Shape*> getShapes();	// returns all of the shapes in the system
	void integrate(float dur);	// manages the physics of the system
	void runPhysics(float dur, int w, int h);	// runs the physics for each shape

	void checkBounds(int pos, float size, float duration, int g, int w, int h);	// checks against the bounds of the window for each shape
protected:
	Shapes available_shapes;	// all of the shapes in the system
	QuadTree* tree;	// a pointer to the quad tree
};