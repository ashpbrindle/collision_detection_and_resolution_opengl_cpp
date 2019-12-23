#ifndef SHAPE_H
#define SHAPE_H

#include "coreMath.h"	// access to Vector2
#include <string>
//#include "contactResolver.h"

/*	Particle class handles the particle(s) themselves and
	allows the ability to change the physics attributes of 
	the particle, including mass and velocity */

class Shape {
protected:
	/* -- Attributes -- */
	float inverse_mass;
	float damping;	// determines how much bounce the particle will have
	Vector2 position;	// holds the position of the particle on the window (x, y)
	Vector2 velocity;	// determines the velocity which the ball is travelling at (x, y)
	Vector2 force_accum;
	Vector2 acceleration;	// the speed in which the particle is moving (x, y)

	/* -- Circle Attributes (Changed by Virtual Methods) -- */
	float radius = 0.0f; //virtual method only used for circle, else 0
	int stacks = 0;
	int slices = 0;

public:
	/* -- Methods -- */
	void integrate(float dur);	// handles the maths behind moving the particle(s), as well as valdidation
	void clearAccumulator();
	void addForce(const Vector2& force);

	/* -- Getters and Setters -- */
	void setMass(const float m);
	float getMass() const;

	void setInverseMass(const float im);
	float getInverseMass() const;

	void setDamping(const float d);
	float getDamping() const;

	void setPosition(const float x, const float y);
	void setPosition(const Vector2 p);
	Vector2 getPosition() const;
	void getPosition(Vector2* p) const;

	void setVelocity(const Vector2 v);
	void setVelocity(const float x, const float y);
	Vector2 getVelocity() const;
	void getVelocity(Vector2* v) const;

	void setAcceleration(const Vector2 acc);
	void setAcceleration(const float x, const float y);
	Vector2 getAcceleration() const;

	//	virtual methods which are set with the circle class
	virtual float getRadius() const { return radius; };
	virtual int getStacks() const { return stacks; };
	virtual int getSlices() const { return slices; };


};

/* The circle class will inherit from the shape class
the radius stacks and slices will be set to the shape 
class with the virtual methods*/

class Circle : public Shape {
private:
	float radius;	// radius of the circle
	int stacks;	// stacks of the circle
	int slices;	// slices of the circle
public:
	Circle(float radius, int stacks, int slices);	// sets all of the circles attributes
	float getRadius() const;
	int getStacks() const;
	int getSlices() const;
};

#endif //SHAPE_H