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

#include "SimObject.h"

using namespace std;

class uav : public SimObject{
private:
	// thrust force of the model
	float thrust = 20.0;
	float Drag_coeff = 5.0;
	// define basic state of the uav
	//glm::vec3 pos = { 0.0, 6.0, 0.1 };
	//glm::vec3 vel = { 0.0, 0.0, 0.0 };

	glm::vec3 twist = { 0.0, 0.0, 0.0};
	glm::vec3 acc = { 0.0, 0.0, 0.0 };
	glm::vec3 cam = { pos.x, pos.y, pos.z };

	// system parameters
	float mass = 3.0; // mass, kg

	// internal and external forces
	glm::vec3 F_motor = { 0.0, 0.0, 0.0 };		// force provided by motors
	glm::vec3 F_drag = { 0.0, 0.0, 0.0 };		// froce by air drag
	glm::vec3 F_coulomb = { 0.0, 0.0, 0.0 };	// Repulsive coulomb force
	glm::vec3 F_join = { 0.0, 0.0, 0.0 };		// join force
	
	// delta time
	float dt = 0.0;

public:
	// constructor inherit
	uav(std::string const& path, glm::vec3 s, glm::vec3 p = glm::vec3(0.0f, 6.0f, 0.1f), glm::vec3 v = glm::vec3(0.0f, 0.0f, 0.0f)) : SimObject(path, s, p, v) {};

	// override draw func
	void Draw(Shader& shader) {
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, pos); // translate
		trans = glm::scale(trans, scalar);

		// additional rotation info
		trans = glm::rotate(trans, twist.z, glm::vec3(cos(twist.y), 0., -sin(twist.y)));
		trans = glm::rotate(trans, twist.x, glm::vec3(sin(twist.y), 0., cos(twist.y)));
		trans = glm::rotate(trans, -1.570796f, glm::vec3(0., 1., 0.));
		trans = glm::rotate(trans, twist.y, glm::vec3(0., 1., 0.));

		shader.setMat4("model", trans);
		ObjectModel.Draw(shader);
	}

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


void uav::forward()
{
	F_motor.x = thrust;
}

void uav::backward()
{
	F_motor.x = -thrust;
}

void uav::left()
{
	F_motor.z = -thrust;
}

void uav::right()
{
	F_motor.z = thrust;
}

void uav::up()
{
	F_motor.y = thrust;
}

void uav::down()
{
	F_motor.y = -thrust;
}

void uav::yawleft()
{
	twist.y += 0.001; // in radians, so it has to be very small
}

void uav::yawright()
{
	twist.y -= 0.001;
}
void uav::hold()
{
	// set motor on hold
	F_motor.x = 0.0;
	F_motor.y = 0.0;
	F_motor.z = 0.0;
}

void uav::dynamics()
{
	// dynamics of uav 
	// get called in main every loop

	// manual "stops" if speed lower than threshould
	if (double(F_motor.x) + double(F_motor.y) + double(F_motor.z) == 0.0) {
		if (abs(vel.x) < 0.15) vel.x = 0;
		if (abs(vel.y) < 0.15) vel.y = 0;
		if (abs(vel.z) < 0.15) vel.z = 0;
	}
	else Drag_coeff = 5.0;

	// Air drag force calculation: 
	F_drag.x = vel.x > 0 ? -Drag_coeff * vel.x * vel.x : Drag_coeff * vel.x * vel.x;
	F_drag.y = vel.y > 0 ? -Drag_coeff * vel.y * vel.y : Drag_coeff * vel.y * vel.y;
	F_drag.z = vel.z > 0 ? -Drag_coeff * vel.z * vel.z : Drag_coeff * vel.z * vel.z;

	F_join.x = F_motor.x + F_drag.x + F_coulomb.x;
	F_join.y = F_motor.y + F_drag.y + F_coulomb.y; // the drone will hover, we are not introducing gravity force
	F_join.z = F_motor.z + F_drag.z + F_coulomb.z;

	// calculate acceleration
	acc.x = F_join.x / mass;
	acc.y = F_join.y / mass;
	acc.z = F_join.z / mass;

	// calculate velocity
	vel.x += acc.x * dt;
	vel.y += acc.y * dt;
	vel.z += acc.z * dt;

	// calculate position
	float temp_x = vel.x * dt;
	float temp_z = vel.z * dt;
	pos.x += temp_x * cos(twist.y) + temp_z * sin(twist.y);
	pos.z += temp_z * cos(twist.y) - temp_x * sin(twist.y);
	pos.y += vel.y * dt;

	 //pos.x += vel.x * dt;
	 //pos.y += vel.y * dt;
	 //pos.z += vel.z * dt;

	// calculate twist angle of the UAV, 
	// which is by definition proportional to the velocity
	twist.x = -0.3 * vel.x;
	twist.z = 0.3 * vel.z;
}

glm::vec3 uav::getUavTwist()
{
	twist.x = -0.3 * vel.x;
	twist.z = 0.3 * vel.z;
	return twist;
}

void uav::switchToFirstPOV()
{
	cam.x = pos.x;
	cam.y = pos.y;
	cam.z = pos.z;
}

void uav::switchToThirdPOV()
{
	cam.x = pos.x + 10;
	cam.y = pos.y + 10;
	cam.z = pos.z + 10;
}
