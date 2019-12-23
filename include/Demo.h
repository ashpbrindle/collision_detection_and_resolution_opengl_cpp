#include <coreMath.h>
#include "simulator.h"

/* Handles what is dispalyed on the window and is used 
to first start the simulation*/

class Demo : public Application {
private:
	Simulator* sim;	// pointer to the simulator class to run the simulation
public:
	Demo();	// shapes get created here
	~Demo();
	void createCircle(int radius, int slice_stacks, Vector2 pos, Vector2 vel, double damping, float mass);	// called to create the shape with the relevent parameters
	void createStillCircle(int r, int ss, Vector2 pos);	// called to create a still shape with the relevent parameters
	void drawCircle(int shape_pos, int r, int g, int b);	// draws the circle shape
	virtual void display();
	virtual void update();

};