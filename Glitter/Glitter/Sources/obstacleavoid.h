#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "flightcontrol.h"


class obstacle{
protected:
	// define basic state of the obstacle
	glm::vec3  pos;   // most likely just position
	glm::vec3  vel;  // velocity for moving obstacles

};

class detector {
private:
	float tolerance;
	uav currUAV;
	// Map for all obstacles inside the tolerance range
	std::vector<obstacle*> obstacleMap;
	glm::vec3 getForwardDir(obstacle* obst, uav* craft);
	
public:
	// check if an obstacle should be in map 
	bool isInMap(obstacle obst, std::vector<obstacle> list, glm::vec3 fwdDir);
	glm::vec3 getRepulse(obstacle* obst, uav* craft);
};