#pragma once
#include "SimObject.h"
#include "flightcontrol.h"


class obstacle : public SimObject{
protected:
	float Radius;

public:
	obstacle();
	//obstacle(Model obstacleModel, glm::vec3  pos, glm::vec3 vel = glm::vec3(0.0f, 0.0f, 0.0f));
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