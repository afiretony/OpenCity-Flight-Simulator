#pragma once
#include "SimObject.h"
#include "flightcontrol.h"



class obstacle : public SimObject{
protected:
	float radius;

public:
	obstacle(float rad, std::string const& path, glm::vec3 s, glm::vec3 p, glm::vec3 v = glm::vec3(0.0f, 0.0f, 0.0f)) : SimObject(path, s, p, v) { radius = rad; };

};

//class detector {
//private:
//	float tolerance;
//
//	uav* myUAV;
//	Map* myCity;
//	//glm::vec3 getForwardDir(obstacle* obst, uav* craft);
//	
//public:
//
//	detector(Map& city, uav& drone);
//
//	//// check if an obstacle should be in map 
//	//bool isInMap(obstacle obst, std::vector<obstacle> list, glm::vec3 fwdDir);
//	//glm::vec3 getRepulse(obstacle* obst, uav* craft);
//};
//
//detector::detector(Map& city, uav& drone) {
//	;
//}