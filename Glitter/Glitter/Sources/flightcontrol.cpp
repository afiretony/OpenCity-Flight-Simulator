#include "flightcontrol.h"

void uav::forward()
{
	pos.y += 1.;
}

void uav::backward()
{
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
	// get called when location of uav updates
	// calculate join force
	F_join.fx = F_motor.fx + F_drag.fx + F_coulomb.fx;
	F_join.fy = F_motor.fy + F_drag.fy + F_coulomb.fy;
	F_join.fz = F_motor.fz + F_drag.fz + F_coulomb.fz;

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

void uav::switchPOV()
{
}
