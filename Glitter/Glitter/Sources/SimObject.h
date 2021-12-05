// OpenCity Simulator
// Group project of 21fall 24780B Engineering Computation
// Carnegie Mellon University
// main function
// more info: https://github.com/afiretony/24780-Engineers-Republic

#ifndef SIMOBJECT_H
#define SIMOBJECT_H

//#include <vector>
//#include <string>
//#include <glm/glm.hpp>

#include <model.h>
#include <cmath>

class SimObject{
protected:
	std::string directory;
	// define basic state of the obstacle
	glm::vec3 pos, vel;
	glm::vec3 scalar;
	float height = 0.0f;
	
public:
	Model ObjectModel;
	SimObject(std::string const& path, glm::vec3 s, glm::vec3 p, glm::vec3 v = glm::vec3(0.0f, 0.0f, 0.0f));
	
	void Draw(Shader& shader)
	{
		//std::cout << "POS:" << pos.x << "\t" << pos.y << "\t" << pos.z << endl;
		//std::cout << "Dir:" << directory << endl;
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::scale(trans, scalar);
		trans = glm::translate(trans, pos); // translate		
		shader.setMat4("model", trans);
		ObjectModel.Draw(shader);
	}

	void UpdateScale(glm::vec3 s);
	void UpdatePos(glm::vec3 p);
	void UpdateVel(glm::vec3 v);
	float GetNormVel();
};

SimObject::SimObject(std::string const& path, glm::vec3 s, glm::vec3 p, glm::vec3 v)
	: ObjectModel(path)
{
	directory = path;
	std::cout << "Load model file from " << directory << std::endl;
	scalar = s;
	pos = p;
	vel = v;
	height = ObjectModel.height;
}

inline void SimObject::UpdateScale(glm::vec3 s)
{
	scalar = s;
}

inline void SimObject::UpdatePos(glm::vec3 p)
{
	pos = p;
}

inline void SimObject::UpdateVel(glm::vec3 v)
{
	vel = v;
}

inline float SimObject::GetNormVel()
{
	float vx = vel.x;
	float vy = vel.y;
	float vz = vel.z;
	float v = sqrtf(vx * vx + vy * vy + vz * vz);
	return v;
}

#endif
