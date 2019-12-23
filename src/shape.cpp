#include "shape.h"
#include <assert.h>
#include <float.h>
#include <iostream>

void Shape::integrate(float dur) {
	// if the shape has infinite mass it will not be processed
	if (inverse_mass <= 0.0f) {
		return;
	}

	// assert checks if the variable is valid
	assert(dur > 0.0);
	// set the acceleration for the shape
	Vector2 resultAcc = acceleration;
	resultAcc.addScaledVector(force_accum, inverse_mass);
	// set the velocity for the shape (with damping included)
	velocity.addScaledVector(resultAcc, dur);
	velocity *= pow(damping, dur);
	position.addScaledVector(velocity, dur);
	clearAccumulator();

}

void Shape::setMass(const float mass) {
	assert(mass != 0);
	Shape::inverse_mass = ((float)1.0) / mass;
}

float Shape::getMass() const {
	if (inverse_mass == 0) {
		return DBL_MAX;
	}
	else {
		// it will return 0 if the mass is infinite
		return ((float)1.0) / inverse_mass;
	}
}

void Shape::setInverseMass(const float im) {
	inverse_mass = im;
}

float Shape::getInverseMass() const {
	return inverse_mass;
}

void Shape::setDamping(const float d) {
	damping = d;
}

float Shape::getDamping() const {
	return damping;
}

void Shape::setPosition(const float x, const float y) {
	position.x = x;
	position.y = y;
}

void Shape::setPosition(const Vector2 pos) {
	position = pos;
}

Vector2 Shape::getPosition() const {
	return position;
}

void Shape::getPosition(Vector2* pos) const {
	*pos = position;
}

void Shape::setVelocity(const float x, const float y) {
	velocity.x = x;
	velocity.y = y;
}

void Shape::setVelocity(const Vector2 v)
{
	velocity = v;
}

Vector2 Shape::getVelocity() const {
	return velocity;
}

void Shape::getVelocity(Vector2* v) const {
	*v = velocity;
}

void Shape::setAcceleration(const Vector2 acc) {
	acceleration = acc;
}

void Shape::setAcceleration(const float x, const float y) {
	acceleration.x = x;
	acceleration.y = y;
}

Vector2 Shape::getAcceleration() const {
	return acceleration;
}

void Shape::clearAccumulator() {
	force_accum.clear();
}

void Shape::addForce(const Vector2& force) {
	force_accum += force;
}

Circle::Circle(float r, int st, int sl) {
	radius = r;
	stacks = st;
	slices = sl;
}

float Circle::getRadius() const {
	return radius;
}

int Circle::getSlices() const {
	return slices;
}

int Circle::getStacks() const {
	return stacks;
}