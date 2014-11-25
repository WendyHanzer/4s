#include "Component.h"

Component::Component(Entity& controller, PulseBehavior<>& componentController, const std::type_index& type, const std::string& name) :
	PulseBehavior(componentController.event),
	controller(controller),
	componentController(componentController),
	actionChangeController(controller.eventChangeController.reg(&Component::onChangeController, this)),
	actionStart(controller.event.start.reg(&Component::start, this)),
	actionStop(controller.event.stop.reg(&Component::stop, this)),
	actionDestroy(controller.event.destroy.reg(&Component::onDestroy, this)),
	name(name),
	mapping(controller.components.context[type])
{
	controller.components.components[(const unsigned int&)mapping] = this;
} //Component

Component::~Component()
{
	actionChangeController->unreg();
	actionStart->unreg();
	actionStop->unreg();
	actionDestroy->unreg();
	
	controller.components.components[mapping] = NULL;
} //~Component

void Component::setController(PulseBehavior<>& componentController)
{
	this->componentController = componentController;
	link(componentController.event);
	eventChangeController.exec();
} //setController
