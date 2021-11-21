#pragma once
#include <vector>
#include <glm/glm.hpp>


class SimObject {
	//private:
	//	Model ObjectModel;

protected:
	// define basic state of the obstacle
	glm::vec3  Pos, Vel;  
	
public:
	SimObject();  // Model objectModel, 
	SimObject(glm::vec3  pos, glm::vec3 vel = glm::vec3(0.0f, 0.0f, 0.0f));
};
