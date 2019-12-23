#include <float.h>
#include <contactResolver.h>
#include <iostream>
#include <coreMath.h>
#include "shape.h"

float ShapeContact::calcSeperatingVelocity() const {
	Vector2 relative_velocity = shapes_colliding[0]->getVelocity();
	if (shapes_colliding[1]) { relative_velocity -= shapes_colliding[1]->getVelocity(); }
	return relative_velocity * contact_normal;
}

void ShapeContact::resolveContactVelocity() {
	//	the seperating velocity is calculated
	float seperating_vel = calcSeperatingVelocity();

	//	if there is not a seperating velocity then it cannot be resolved
	if (seperating_vel > 0) { return; }

	//	the new seperating velocity is then calculated
	float new_seperating_vel = -seperating_vel * contact_restitution;
	float delta_vel = new_seperating_vel - seperating_vel;

	//	the mass of the object is then checked as objects with infinite mass will not change in velocity
	float total_inverse_mass = shapes_colliding[0]->getInverseMass();
	if (shapes_colliding[1]) total_inverse_mass += shapes_colliding[1]->getInverseMass();

	if (total_inverse_mass <= 0) { return; }	// return if the mass is infinite

	float impulse = delta_vel / total_inverse_mass;
	Vector2 impluse_per_Imass = contact_normal * impulse;

	//	the change in velocity is then applied to each shape
	shapes_colliding[0]->setVelocity(shapes_colliding[0]->getVelocity() + impluse_per_Imass * shapes_colliding[0]->getInverseMass());
	if (shapes_colliding[1]) { shapes_colliding[1]->setVelocity(shapes_colliding[1]->getVelocity() + impluse_per_Imass * -shapes_colliding[1]->getInverseMass()); }

	//	the penetration will then be resolved
	this->resolvePenetration();
}

void ShapeContact::resolvePenetration() {
	//	if there is no penetration then it will not need to be resolved
	if (depth_penetration <= 0) {
		return;
	}

	//	get the inverse mass for the objects and check if the mass is infnite (0)
	float totalInverseMass = shapes_colliding[0]->getInverseMass();
	if (shapes_colliding[1]) {
		totalInverseMass += shapes_colliding[1]->getInverseMass();
	}
	if (totalInverseMass <= 0) {
		return;
	}

	//	calculate the penetration resolution for each unit of inverse mass
	Vector2 movement2;
	Vector2 move_per_inverse_mass = contact_normal * (depth_penetration / totalInverseMass);
	Vector2 movement1 = move_per_inverse_mass * shapes_colliding[0]->getInverseMass();

	if (shapes_colliding[1]) {
		movement2 = move_per_inverse_mass * -shapes_colliding[1]->getInverseMass();
	}
	else {
		movement2.clear();
	}

	//	resolve the penetration for each shape
	shapes_colliding[0]->setPosition(shapes_colliding[0]->getPosition() + movement1);
	if (shapes_colliding[1]) {
		shapes_colliding[1]->setPosition(shapes_colliding[1]->getPosition() + movement2);
	}
}

CollisionManager::CollisionManager() {
	contacts = new ShapeContact[2];
}

//	this is called when a contact is found
void CollisionManager::addContact(ShapeContact* contact, Shape* s1, Shape* s2) const {
	const static float restitution = 1.0f;
	float penetration = 0;

	//	calculates the distance between the 2 objects to help determine 
	//		the penetration to be resolved
	float distance = (s1->getPosition() - s2->getPosition()).magnitude();
	if (s1->getRadius() > 0 && s2->getRadius() > 0) {
		//	radius combo is used to check if the distance and the 
		//		radius_combo intersect, causing a penetration
		float radius_combo = s1->getRadius() + s2->getRadius();
		penetration = radius_combo - distance;
	}

	//	the normal is calculated and then normalised so 
	//		it can be used to resolve the contact
	Vector2 normal = s1->getPosition() - s2->getPosition();
	normal.normalise();

	//	all of the needed attributes are set for the contact so it can be resolved
	contact->contact_normal = normal;
	contact->contact_restitution = restitution;
	//	assigns the 2 shapes which are colliding
	contact->shapes_colliding[0] = s1;
	contact->shapes_colliding[1] = s2;
	contact->depth_penetration = penetration;
}

void CollisionManager::checkCollisionsQuadTree(QuadTree* tree, std::vector<Shape*> available_shapes) {
	//	a vector to hold all of the shapes which are in the same node as the current shape is
	std::vector <Shape*> node_shapes;

	//	searches for the current shape in each node and returns possible collisions
	for (int i = 0; i < available_shapes.size() - 1; i++) {
		//	returns possible collisions here
		node_shapes = tree->retrieve(available_shapes[i]);

		//	if there is enough shapes for a collision check for a collision
		if (node_shapes.size() >= 2) {
			int s1_size = 0;
			int s2_size = 0;

			//	checks the first shape against all other shapes in that node
			//		and keep checking until all shapes have been checked for collisions
			for (int s1 = 0; s1 < node_shapes.size(); s1++) {

				//	gets the positions for the current node
				float s1_x = node_shapes[s1]->getPosition().x;
				float s1_y = node_shapes[s1]->getPosition().y;

				//	checks against the next node, s2 is s1 here to stop the 
				//		system checking against nodes it has already checked 
				//		against, to speed up computation time
				for (int s2 = s1 + 1; s2 < node_shapes.size(); s2++) {
					//	gets the positions for the checking node
					float s2_x = node_shapes[s2]->getPosition().x;
					float s2_y = node_shapes[s2]->getPosition().y;
					//	checks if the nodes are the same, if they are not, do not compare them

					//	get distance between shapes
					float distance = sqrt(pow((s1_x - s2_x), 2) + pow((s1_y - s2_y), 2));

					s1_size = node_shapes[s1]->getRadius();
					s2_size = node_shapes[s2]->getRadius();

					//	checks if the shapes collide or intercept
					if (distance <= s1_size + s2_size) {

						//	then add the contact to the Shape Contact with the relevent 
						//		attributes to solve the collision
						addContact(contacts, node_shapes[s1], node_shapes[s2]);
						//	resolve the collision for this contact
						contacts->resolveContactVelocity();
					}
				}
			}
		}

	}
}

CollisionManager::~CollisionManager() {
	delete contacts;
}