#ifndef CONTACTRESOLVER_H
#define CONTACTRESOLVER_H
#pragma once
#include <shape.h>
#include "quadTree.h"
#include <vector>

/* Handles the contact for the shapes and the resolution. */

class ShapeContact {
public:
	/* -- Attributes -- */
	float contact_restitution;	// holds the normal restitution of the contacted particles (energy from contact)
	Shape* shapes_colliding[2];	// when collision occurs the 2 colliding particles will be stored in this array
	Vector2 contact_normal;	// holds the world cooridnates for the direction of contact
	float depth_penetration;	// depth of penetration for the contact
	void resolveContactVelocity();	// resolves this contact (velocity)
	void resolvePenetration();	// resolves the penetration

protected:
	float calcSeperatingVelocity() const;	// used to calculate the seperating velocity
};

/* Handles the creating the contact for the shape with 
collision detection. Then resolving the contact with the 
ShapeContact Class*/

class CollisionManager {
public:
	CollisionManager();	// initalises the contact
	~CollisionManager();	// destroys the contact
	void addContact(ShapeContact* contact, Shape* s1, Shape* s2) const;	// creates and adds the contact
	void checkCollisionsQuadTree(QuadTree* tree, std::vector<Shape*> available_shapes);	//	checks for collisions
protected:
	ShapeContact* contacts;	// stores the contact
};

#endif CONTACTRESOLVER_H
