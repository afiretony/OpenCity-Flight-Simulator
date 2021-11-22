#ifndef SIMOBJECT_H
#define SIMOBJECT_H

//#include <vector>
//#include <string>
//#include <glm/glm.hpp>

#include <model.h>

class SimObject{
protected:
	std::string directory;
	// define basic state of the obstacle
	glm::vec3 pos, vel;
	glm::vec3 scalar;
	
public:
	Model ObjectModel;
	SimObject(std::string const& path, glm::vec3 s, glm::vec3 p, glm::vec3 v = glm::vec3(0.0f, 0.0f, 0.0f));
	
	void Draw(Shader& shader)
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, pos); // translate
		trans = glm::scale(trans, scalar);
		shader.setMat4("model", trans);
		ObjectModel.Draw(shader);
	}

	void UpdateScale(glm::vec3 s);
	void UpdatePos(glm::vec3 p);
	void UpdateVel(glm::vec3 v);
};

SimObject::SimObject(std::string const& path, glm::vec3 s, glm::vec3 p, glm::vec3 v)
	: ObjectModel(path)
{
	directory = path;
	std::cout << "Load model file from " << directory << std::endl;
	scalar = s;
	pos = p;
	vel = v;
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

#endif