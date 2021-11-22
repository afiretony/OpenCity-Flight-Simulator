#include "flightcontrol.h"

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
	F_motor.z = thrust;
}

void uav::right()
{
	F_motor.z = -thrust;
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
	// for update
}

void uav::yawright()
{
	// for update
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
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;
	pos.z += vel.z * dt;

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
