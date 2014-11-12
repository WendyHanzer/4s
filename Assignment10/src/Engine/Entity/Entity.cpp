#include "Entity.h"

Entity::Entity(ControlBehavior<>& controller, EntityManager& manager, const std::string& name) :
	ControlBehavior(controller.event),
	controller(&controller),
	manager(&manager),
	actionStart(manager.event.start.reg(&Entity::start, this)),
	actionStop(manager.event.stop.reg(&Entity::stop, this)),
	actionDestroy(manager.event.destroy.reg(&Entity::onDestroy, this)),
	name(name),
	id(manager.idManager.allocate()),
	components(manager.context)
{
} //Entity

Entity::Entity(EntityManager& manager, const std::string name) :
	ControlBehavior(manager.event),
	controller(NULL),
	manager(&manager),
	actionStart(NULL),
	actionStop(NULL),
	actionDestroy(NULL),
	name(name),
	id(manager.idManager.allocate()),
	components(manager.context)
{
} //Entity

Entity::~Entity()
{
	if (controller)
	{
		actionStart->unreg();
		actionStop->unreg();
		actionDestroy->unreg();
	} //if
	
	getManager().idManager.deallocate(id);
} //~Entity

void Entity::setController(ControlBehavior<>& controller)
{
	if (this->controller)
	{
		this->controller = &controller;
		link(controller.event);
		eventChangeController.exec();
		
		if (!controller.isRunning())
		{
			stop();
		} //if
	} //if
} //setController
