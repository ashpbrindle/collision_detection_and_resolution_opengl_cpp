#include <vector>
#include "shape.h"

class QuadTree {
public:
	QuadTree(float x, float y, float width, float height, int level, int max_level);	// creates a quad tree with 4 sub nodes
	~QuadTree();	// detroys the tree and its sub nodes
	void addShape(Shape* sh);	// adds a shape to the quadtree
	std::vector<Shape*> retrieve(Shape* sh);	//	retrieves the shapes which could collide with the passed in shape
	void clear(int w, int h);	// clears the tree and re-initalises it with the new window dimensions

	int size;	// size of the trees shapes

private:
	float x;	// bottom left co-ordinates of the top node or sub nodes
	float y;	// ^
	float width;	//	width and height of the top node (window) and its sub nodes
	float height;
	int level;	// current level of this tree, 0 lowest, 1 max
	int max_level;	// max level set to 1 on initalise

	std::vector<Shape*> shapes_in_node;	// holds all of the shapes in current node, top or sub nodes

	QuadTree* NW;	// NW node
	QuadTree* NE;	// NE node
	QuadTree* SW;	//SW node
	QuadTree* SE;	//SE node

	Shape shape;

	bool detect(QuadTree* sub_node, Shape* sh);	// detects where the shape is located (NW, NE, SW, SE)

};