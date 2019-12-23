#pragma once

/*	Application class handles the application itself and
	allows access to the attirbutes of the window and if
	any were to change, for example; changing particle 
	position when the window resizes and displaying objects 
	onto the window */

class Application {
protected:
	/* -- Attributes -- */
	int h;	// holds the height of the window
	int w;	// holds the width of the window
	float n_range;	// n_range determines how the distance from the centre of the window to the edge is messured
	float time_interval;

public:
	/* -- Virtual Methods -- */
	virtual void initGraphics(float r, float g, float b);
	virtual void display();
	virtual void update();
	virtual void resize(int w, int h);

	/* -- Getters and Setters -- */
	int getHeight() { return h; }
	int getWidth() { return w; }
	float getTimeInterval() { return time_interval; }
	void setTimeInterval(float t) { time_interval = t; }
};