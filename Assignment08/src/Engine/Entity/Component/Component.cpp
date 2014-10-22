#include "Component.h"

Component::Component(Entity& controller, PulseBehavior& componentController, const std::type_index& type):
	PulseBehavior(componentController.event),
	controller(controller),
	componentController(componentController),
	actionChangeController(controller.event.start.reg(&Component::onChangeController, this)),
	actionStart(controller.event.start.reg(&Component::start, this)),
	actionStop(controller.event.stop.reg(&Component::stop, this)),
	actionDestroy(controller.event.destroy.reg(&Component::onDestroy, this)),
	mapping(controller.components.getMapping(type))
{
	controller.components[mapping] = this;
} //Aspect

Component::~Component()
{
	actionChangeController->unreg();
	actionStart->unreg();
	actionStop->unreg();
	actionDestroy->unreg();
	
	controller.components[mapping] = NULL;
} //~Aspect
