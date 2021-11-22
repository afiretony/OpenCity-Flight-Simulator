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
	SimObject(std::string const& path, glm::vec3& s, glm::vec3&  p, glm::vec3& v = glm::vec3(0.0f, 0.0f, 0.0f));
	void Draw(Shader& shader)
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, pos); // translate
		trans = glm::scale(trans, scalar);
		shader.setMat4("model", trans);
		ObjectModel.Draw(shader);
	}
};

SimObject::SimObject(std::string const& path, glm::vec3& s, glm::vec3& p, glm::vec3& v)
	: ObjectModel(path)
{
	directory = path;
	std::cout << "Load model file from " << directory << std::endl;
	scalar = s;
	pos = p;
	vel = v;
}

#endif