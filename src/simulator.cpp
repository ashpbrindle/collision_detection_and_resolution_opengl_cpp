#include "simulator.h"

int const MAX_QUAD_TREE_LEVEL = 1;	//	 this implementation of a quad tree only supports levels 0 and 1 to work correctly

Simulator::Simulator(float w, float h, float n_range) {
	// creates a new quad tree to fill all of the shapes into
	// width and height are set to the n_range * 2 as the bounds of the window range from (-100 -> 100)
	// while the x and y are set to the bottom corner of the window
	// the current level is 0 as this is the top level of the tree
	
	// the max level is 1 as this implementation of a quad tree only supports 1 level
	tree = new QuadTree(-n_range, -n_range, n_range * 2, n_range * 2, 0, MAX_QUAD_TREE_LEVEL);
}

Simulator::~Simulator() {
	// deletes all of the pointers associated with the simulator
	for (int shape = 0; shape < available_shapes.size(); shape++) {
		delete available_shapes[shape];
	}
	delete tree;
}

std::vector<Shape*> Simulator::getShapes() {
	return available_shapes;
}

void Simulator::addShape(Shape* sh) {
	available_shapes.push_back(sh);
}

// the run physics method handles the physics for the application on each shape
void Simulator::runPhysics(float dur, int w, int h) {
	// applies the forces
	integrate(dur);

	//	clears the quad tree to reinitalise with different shape positions

	//	this also assigns a new width and height to the tree incase the window 
	//		has been resized (allows for shapes to collide in the new available areas)
	tree->clear(w, h);

	//	 once the tree has cleared itself, it is ready to be repopulated with the new shape positions
	for (int i = 0; i < available_shapes.size(); i++) {
		tree->addShape(available_shapes[i]);
	}

	//	once all of the shapes have been added to the tree in the correct nodes, 
	//		the collision manager will then check each shape against the others in that node
	manager.checkCollisionsQuadTree(tree, available_shapes);
}

void Simulator::integrate(float dur) {
	//	runs through each of the shapes and applies the relevent forces 
	//		to each of the shapes and moves it to the correct position
	for (Shapes::iterator shape = available_shapes.begin(); shape != available_shapes.end(); shape++) {
		(*shape)->integrate(dur);
	}
}

//	gets called from the Demo class to check the bounds of the window agaisnt each of the shapes
void Simulator::checkBounds(int pos, float size, float dur, int g, int w, int h) {
	Shape* shape = available_shapes[pos];

	Vector2 position = (shape)->getPosition();
	Vector2 velocity = (shape)->getVelocity();

	//	checks if the shape is within the bounds of the window
	//		and if it is colliding or intersecting with the bound, the shapes velocity will then be resolved
	//		for (x) and the (width)
	if (position.x > w - size || position.x < -w + size) {
		(shape)->setVelocity(-velocity.x, velocity.y);
	}

	//		and for (y) and the (height)
	if (position.y > h - size || position.y < -h + size) {
		(shape)->setVelocity(velocity.x, -velocity.y);
	}

	//	This then checks if there is any penetration through the 
	//		window which needs to be resolved, usually occurs when the 
	//		object is still and colliding with the bounds. This will then 
	//		push the object to the edge of the window and set its position
	if (position.x > (w - size)) {
		position.x = w - size;
	}
	else if (position.x < -w + size) {
		position.x = -w + size;
	}

	if (position.y > (h - size)) {
		position.y = h - size;
	}
	else if (position.y < -h + size) {
		position.y = -h + size;
	}
	(shape)->setPosition(position.x, position.y);
}