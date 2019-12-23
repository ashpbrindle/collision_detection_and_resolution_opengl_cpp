#include "quadTree.h"
#include <algorithm>
#include <iostream>
#include <stdlib.h>

QuadTree::QuadTree(float x, float y, float width, float height, int level, int max_level) {
	//	assigns the attributes to the top of the tree
	//		starting with the x and y coordinates being the bottom 
	//		left of the top node (-100, -100 on start)
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->level = level;
	this->max_level = max_level;
	//	this will only run when the max level has not been reached (set to 1)
	if (level != max_level) {
		//	this will then set the coordinates and width/height for each of the sub nodes 
		//		to be added to the tree, with all increasing in level as the tree grows
		NW = new QuadTree(x, y + (height / 2), width / 2, height / 2, level + 1, max_level);
		NE = new QuadTree(x + (width / 2), y + (height / 2), width / 2, height / 2, level + 1, max_level);
		SW = new QuadTree(x, y, width / 2, height / 2, level + 1, max_level);
		SE = new QuadTree(x + (width / 2), y, width / 2, height / 2, level + 1, max_level);
	}
}

QuadTree::~QuadTree() {
	NW = nullptr;
	NE = nullptr;
	SW = nullptr;
	SE = nullptr;
}

//	used to determine the location in the bounds 
//		for each shape passed in
void QuadTree::addShape(Shape* shape) {

	//	checks if the level is the max level (1), 
	//		prevents the shape being added to the top node
	if (level >= max_level) {
		shapes_in_node.push_back(shape);
	}
	else {
		/*std::cout << shape << " Located: ";*/

		//	detects if the node is located in the north west of the window bounds
		//	calls the add shape method on the north west node, 
		//		to add the shape to this node
		if (detect(NW, shape)) 	NW->addShape(shape);	/*std::cout << "NW ";*/

		//	detects if the node is located in the north east of the window bounds
		//	calls the add shape method on the north east node, 
		//		to add the shape to this node
		if (detect(NE, shape)) NE->addShape(shape);	/*std::cout << "NE ";*/

		//	detects if the node is located in the south east of the window bounds
		//	calls the add shape method on the south east node, 
		//		to add the shape to this node
		if (detect(SE, shape)) SE->addShape(shape);	/*std::cout << "SE ";*/

		//	detects if the node is located in the south west of the window bounds
		//	calls the add shape method on the south west node, 
		//		to add the shape to this node
		if (detect(SW, shape)) SW->addShape(shape);	/*std::cout << "SW ";*/
		/*std::cout << std::endl;*/
	}
}

bool QuadTree::detect(QuadTree* sub_node, Shape* shape) {
	//	the radius * 2 will represent how far out of its current node the system can check
	//		this allows for the shape to still be within the node even if it is 
	//		sligthly intercepting the border
	float bounding_box_extension = shape->getRadius() * 2;

	// checks for the negative side of the node + the extension
	if (shape->getPosition().x < sub_node->x - bounding_box_extension ||
		shape->getPosition().y < sub_node->y - bounding_box_extension) {
		return false;
	}

	// checks for the positive side of the node + the extension (x/y + width/height travels to the positive side)
	if (shape->getPosition().x > sub_node->x + sub_node->width + bounding_box_extension ||
		shape->getPosition().y > sub_node->y + sub_node->height + bounding_box_extension) {
		return false;
	}

	// returns true if the shape is within the node + the extension
	return true;

	//if (shape->getPosition().x > sub_node->x + sub_node->width ||
	//	shape->getPosition().y > sub_node->y + sub_node->height) {
	//	return false;
	//}
}

std::vector<Shape*> QuadTree::retrieve(Shape* shape) {
	std::vector<Shape*> possible_collisions;
	//	adds the current shape to the vector of collisions so it can be used to 
	//		compare against the other shapes that could collide
	possible_collisions.push_back(shape);
	//	if statement to see if it is possible for there to be sub nodes
	if (max_level != 0) {
		//	searches for the shape in each of the nodes in the tree, NW, NE, SW, SE using an iterator.
		if (std::find(NW->shapes_in_node.begin(), NW->shapes_in_node.end(), shape) != NW->shapes_in_node.end()) {
			//	once found the other shapes in that node will be added to another vector and 
			//		be classed as a possible collision. This for loop also stops duplicates 
			//		from being added to the collisions vector
			for (int i = 0; i < NW->shapes_in_node.size(); i++) {
				if (NW->shapes_in_node[i] != shape) possible_collisions.push_back(NW->shapes_in_node[i]);
			}
		}
		if (std::find(NE->shapes_in_node.begin(), NE->shapes_in_node.end(), shape) != NE->shapes_in_node.end()) {
			for (int i = 0; i < NE->shapes_in_node.size(); i++) {
				if (NE->shapes_in_node[i] != shape) possible_collisions.push_back(NE->shapes_in_node[i]);
			}
		}
		if (std::find(SW->shapes_in_node.begin(), SW->shapes_in_node.end(), shape) != SW->shapes_in_node.end()) {
			for (int i = 0; i < SW->shapes_in_node.size(); i++) {
				if (SW->shapes_in_node[i] != shape) possible_collisions.push_back(SW->shapes_in_node[i]);
			}
		}
		if (std::find(SE->shapes_in_node.begin(), SE->shapes_in_node.end(), shape) != SE->shapes_in_node.end()) {
			for (int i = 0; i < SE->shapes_in_node.size(); i++) {
				if (SE->shapes_in_node[i] != shape) possible_collisions.push_back(SE->shapes_in_node[i]);
			}
		}
	}
	else {
		for (int i = 0; i < this->shapes_in_node.size(); i++) {
			if (this->shapes_in_node[i] != shape) possible_collisions.push_back(this->shapes_in_node[i]);
		}
	}

	//	returns the collisions vector including all of the possible collisions this 
	//		shape can collide with
	return possible_collisions;
}
 
//	clears the tree to be used again with new positioning
void QuadTree::clear(int w, int h) {

	//	clears its own attributes and re-assigns new ones if changed
	this->shapes_in_node.clear();
	this->x = -w;
	this->y = -h;
	this->width = w * 2;
	this->height = h * 2;
	this->level = 0;

	//	if statement to see if it is possible for there to be sub nodes
	if (max_level != 0) {
		//	clears all of the shapes in each of the nodes
		NW->shapes_in_node.clear();
		NE->shapes_in_node.clear();
		SW->shapes_in_node.clear();
		SE->shapes_in_node.clear();

		//	initalises the sub nodes again with the potential new dimentions 
		//		(same as in the constructor, just with the new values)
		NW->x = this->x;
		NW->y = this->y + (this->height / 2);
		NW->width = this->width / 2;
		NW->height = this->height / 2;

		NE->x = this->x + (this->width / 2);
		NE->y = this->y + (this->height / 2);
		NE->width = this->width / 2;
		NE->height = this->height / 2;

		SW->x = this->x;
		SW->y = this->y;
		SW->width = this->width / 2;
		SW->height = this->height / 2;

		SE->x = this->x + (this->width / 2);
		SE->y = this->y;
		SE->width = this->width / 2;
		SE->height = this->height / 2;
	}
}



