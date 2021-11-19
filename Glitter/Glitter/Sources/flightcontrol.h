#pragma once
#include<math.h>
#include<iostream>
#include <vector>
#include <glm/glm.hpp>

#define PI 3.14159265
using namespace std;

struct position {
	// position of the uav
	float x;
	float y;
	float z;
};

struct velocity {
	// velocity of the uav
	float vx;
	float vy;
	float vz;
};
 
struct acceleration {
	// acceleration of the uav
	float ax;
	float ay;
	float az;
};

struct generalforce {
	float fx;
	float fy;
	float fz;
};

struct cameraPosition {
	float Px;
	float Py;
	float Pz;
};

// comment added by shaobo: consider using glm::vec3 instead of 
// self-defined structure for future vec calculations

class uav {
private:
	// thrust force of the model
	float thrust = 50.0;
	float Drag_coeff = 5.0;
	// define basic state of the uav
	position pos = { 0.0, 0.0, 0.1 };
	velocity vel = { 0.0, 0.0, 0.0 };
	acceleration acc = { 0.0, 0.0, 0.0 };
	cameraPosition cam = { pos.x, pos.y, pos.z };

	// system parameters
	float mass = 3.0; // mass, kg
	float dt = 1.0;

	// internal and external forces
	generalforce F_motor = { 0.0, 0.0, 0.0 };	// force provided by motors
	generalforce F_drag = { 0.0, 0.0, 0.0 };	// froce by air drag
	generalforce F_coulomb = { 0.0, 0.0, 0.0 }; // Repulsive coulomb force
	generalforce F_join = { 0.0, 0.0, 0.0 };	// join force

public:
	// basic controls
	void forward();
	void backward();
	void left();
	void right();
	void up();
	void down();
	void yawleft();
	void yawright();

	// calculate drone's current join force and accerelation
	void dynamics();

	// Added: get uav position
	glm::vec3 getUavPos();
	void switchToFirstPOV();
	void switchToThirdPOV();
};