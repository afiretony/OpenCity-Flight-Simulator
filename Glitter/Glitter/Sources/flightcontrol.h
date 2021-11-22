// Head Maintainer: Chenhao Yang [yangchenhao@cmu.edu]
// Collaborator(if any): Shaobo Wang, Wei Wu
// 
// Group project of 21fall 24780B Engineering Computation
// Carnegie Mellon University
// 
// "flightcontorl.h" "flightcontorl.cpp"
// functions:
// 1. stores state(position, velocity, acceleration) of UAV
// 2. calculates dynamics of UAV
// 3. provides function calls for keyboard contorl
//

#pragma once
#include<math.h>
#include<iostream>
#include <vector>
#include <glm/glm.hpp>

using namespace std;

// comment added by shaobo: consider using glm::vec3 instead of 
// self-defined structure for future vec calculations

class uav {
private:
	// thrust force of the model
	float thrust = 20.0;
	float Drag_coeff = 5.0;
	// define basic state of the uav
	glm::vec3 pos = { 0.0, 6.0, 0.1 };
	glm::vec3 twist = { 0.0, 0.0, 0.0 };
	glm::vec3 vel = { 0.0, 0.0, 0.0 };
	glm::vec3 acc = { 0.0, 0.0, 0.0 };
	glm::vec3 cam = { pos.x, pos.y, pos.z };

	// system parameters
	float mass = 3.0; // mass, kg

	// internal and external forces
	glm::vec3 F_motor = { 0.0, 0.0, 0.0 };	// force provided by motors
	glm::vec3 F_drag = { 0.0, 0.0, 0.0 };	// froce by air drag
	glm::vec3 F_coulomb = { 0.0, 0.0, 0.0 }; // Repulsive coulomb force
	glm::vec3 F_join = { 0.0, 0.0, 0.0 };	// join force
	
	// delta time
	float dt = 0.0;

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
	void hold();
	
	void getdeltatime(float deltatime) { dt = deltatime; }
	void setCoulombF(glm::vec3 vec) { F_coulomb = vec; }
	// calculate drone's current join force and accerelation
	void dynamics();

	// Added: get uav position
	glm::vec3 getUavPos() { return glm::vec3(pos.x, pos.y, pos.z); };
	glm::vec3 getUavTwist();

	void switchToFirstPOV();
	void switchToThirdPOV();
};