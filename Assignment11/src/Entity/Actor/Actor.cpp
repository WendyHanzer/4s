#include "Actor.h"

Actor::Actor(ActorManager& actorManager, const std::string name) :
	Entity(actorManager, name),
	transformation(1.0f)
{
} //Actor

Actor::~Actor()
{
	destroy();
} //~Actor

void Actor::setTransformation(const glm::mat4& transformation)
{
	this->transformation = transformation;

	eventTransformationChange.exec();
} //setTransformation
