#include "AspectSource.h"

Entity::Entity(ControlBehavior& controller, EntityManager& manager, const std::string name) :
	ControlBehavior(controller.event),
	controller(&controller),
	manager(&manager),
	actionStart(manager.event.start.reg(&Entity::start, this)),
	actionStop(manager.event.stop(&Entity::stop, this)),
	actionDestroy(manager.event.destroy(&Entity::onDestroy, this)),
	name(name),
	id(manager.idManager.allocate())
{
} //Entity

Entity::~Entity()
{
	actionStart->unreg();
	actionStop->unreg();
	actionDestroy->unreg();
	
	manager.idManager.deallocate(id);
} //~Entity
