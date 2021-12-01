#ifndef DETECTOR_H
#define DETECTOR_H

#include "map.h"
#include "obstacleavoid.h"

class detector {
private:
	const float tolerance = 0.08;

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
	if (!isAbove()) {
		myUAV->setCoulombF(getRepulse());
	}
}

inline bool detector::isAbove()
{
	bool aboveFlag = true;
	auto neighbour = myCity->getNeighbour(myUAV->getUavPos());
	float uav_height = myUAV->getUavPos().y;
	for (auto& block : neighbour) {
		aboveFlag = aboveFlag && (block.block_height <= uav_height);
	}
	return aboveFlag;
}

inline glm::vec3 detector::getRepulse()
{
	return glm::vec3(0.0f);
}

#endif // DETECTOR_H




