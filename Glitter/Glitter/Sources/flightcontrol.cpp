#include "flightcontrol.h"

void uav::forward()
{
	F_motor.fx = thrust;
}

void uav::backward()
{
	F_motor.fx = -50.0;
}

void uav::left()
{
}

void uav::right()
{
}

void uav::up()
{
}

void uav::down()
{
}

void uav::yawleft()
{
}

void uav::yawright()
{
}

void uav::dynamics()
{
	// dynamics of uav 
	// get called in main every loop
	
	// Air drag force calculation: 
	F_drag.fx = -Drag_coeff * vel.vx * vel.vx;
	F_drag.fy = -Drag_coeff * vel.vy * vel.vy;
	F_drag.fz = -Drag_coeff * vel.vz * vel.vz;

	F_join.fx = F_motor.fx + F_drag.fx + F_coulomb.fx;
	F_join.fy = F_motor.fy + F_drag.fy + F_coulomb.fy;
	F_join.fz = F_motor.fz + F_drag.fz + F_coulomb.fz; // the drone will hover, we are not introducing gravity force

	// calculate acceleration
	acc.ax = F_join.fx / mass;
	acc.ay = F_join.fy / mass;
	acc.az = F_join.fz / mass;

	// calculate velocity
	vel.vx += acc.ax * dt;
	vel.vy += acc.ay * dt;
	vel.vz += acc.az * dt;

	// calculate position
	pos.x += vel.vx * dt;
	pos.y += vel.vy * dt;
	pos.z += vel.vz * dt;

}

glm::vec3 uav::getUavPos()
{
	return glm::vec3();
}

void uav::switchToFirstPOV()
{
	cam.Px = pos.x;
	cam.Py = pos.y;
	cam.Pz = pos.z;
}

void uav::switchToThirdPOV()
{
	cam.Px = pos.x + 10;
	cam.Py = pos.y + 10;
	cam.Pz = pos.z + 10;
}
