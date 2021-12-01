#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "SimObject.h"
#include "flightcontrol.h"



class obstacle : public SimObject{
protected:
	float radius;

public:
	obstacle(float rad, std::string const& path, glm::vec3 s, glm::vec3 p, glm::vec3 v = glm::vec3(0.0f, 0.0f, 0.0f)) : SimObject(path, s, p, v) { radius = rad; };

};

#endif