# include "SimObject.h"

SimObject::SimObject()
{

}

SimObject::SimObject(Model objectModel, glm::vec3 pos, glm::vec3 vel)
{
	ObjectModel = objectModel;
	Pos = pos;
	Vel = vel;
}
