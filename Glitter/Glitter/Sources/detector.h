#ifndef DETECTOR_H
#define DETECTOR_H

#include "map.h"
#include "obstacleavoid.h"

class detector {
private:
	const float tolerance = 4;

	uav* myUAV;
	Map* myCity;
	
public:

	detector(uav* drone, Map* city);

	// avoid logic
	void uav_control();

	// tell if uav is above neighbour obstacles
	bool isAbove();

	// get repulse for uav control
	glm::vec3 getRepulse();
};

detector::detector(uav* drone, Map* city) {
	myUAV = drone;
	myCity = city;
}

inline void detector::uav_control()
{	
	if (myUAV->getUavPos().y < 0.1f)
	{
		myUAV->up();
		myUAV->dynamics();
	}
	else if (!isAbove()) {
		myUAV->setCoulombF(getRepulse());
	}
	else {
		myUAV->setCoulombF(glm::vec3(0.0f));
	}
}

inline bool detector::isAbove()
{
	bool aboveFlag = true;	
	float uav_height = myUAV->getUavPos().y;
	auto neighbour = myCity->getNeighbour(myUAV->getUavPos());
	//for (auto& grid : neighbour) {
	if(neighbour.size() != 0)
		aboveFlag = aboveFlag && (neighbour.at(0).block.block_height <= uav_height);
	//}
	//std::cout << "aboveFlag: " << aboveFlag << std::endl;
	return aboveFlag;
}

inline glm::vec3 detector::getRepulse()
{
	glm::vec3 force = glm::vec3(0.0f);
	auto neighbour = myCity->getNeighbour(myUAV->getUavPos());
	auto nearest = neighbour.at(0).block;
	float scale = myCity->scale;
	float len = myCity->grid_len * scale;
	glm::vec2 obstacle_pos = glm::vec2(scale * nearest.coord.x, scale * nearest.coord.z);
	glm::vec2 uav_pos = glm::vec2(myUAV->getUavPos().x, myUAV->getUavPos().z);
	float h_dist = glm::length(obstacle_pos - uav_pos);
	std::cout << "h_dist-len: " << h_dist - len << std::endl;
	if ((h_dist - len) <= tolerance) {
		glm::vec2 dir = glm::normalize(uav_pos - obstacle_pos); 
		float mag = 800.0f / (h_dist * h_dist);
		//float mag = 10.0f;
		force += glm::vec3((mag * dir).x, 0.0f, (mag * dir).y);
		//std::cout << "force: " << force.x << "," << force.y << "," << force.z << std::endl;
	}
	//else if ((h_dist - len) <= 0)
	//{
	//	glm::vec2 dir = glm::normalize(uav_pos - obstacle_pos);
	//	//float mag = 100.0f / (h_dist * h_dist);
	//	float mag = 1000.0f;
	//	force += glm::vec3((mag * dir).x, 0.0f, (mag * dir).y);
	//	std::cout << "force: " << force.x << "," << force.y << "," << force.z << std::endl;
	//}
	else{
		force = glm::vec3(0.0f);
	}
	return force;
}

#endif // DETECTOR_H




