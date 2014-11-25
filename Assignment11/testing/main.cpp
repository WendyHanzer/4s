/*
For Entity Aspect

	Scene -> Pulse				Arg1
	EntityManager -> Type		Arg2
	Entity -> Source			Arg3

For Entity Aspect Source

	SceneNode -> Controller		Arg1
	EntityManager -> Manager	Arg2
*/

/*
Scene Graphics Component

	Vector of Cameras
	When put on Layer, tell Layer which Camera to use
	Each Camera has an origin camera
	If origin Camera is NULL, the camera is absolute
	If origin camera is not NULL, the camera is a transformation of the origin camera
*/

/*
SceneManager
	ComponentContext


Position(Actor, Origin)

OnChangeOrigin

Entities
	onControllerChange	//Pulse behavior
	getController()		//Pulse behavior


	Position()
		controls the controller!


	eventControllerChange
	getController

Model
	Mesh
	Texture[] (layers)
	Transformations, etc


Entity
	start/stop/destroy from controller
*/

#include <iostream>

#include "Engine/Engine.h"
#include "Engine/System/Manager.h"
#include "Engine/System/System.h"
#include "Engine/Entity/Entity.h"

using namespace std;

/*
*	This is a standard data manager. It just does conditional
*	loading of data. This class is specialized only for this.
*
*	The first value in the template is the type of value that the
*	manager manages. The rest of the values (as many as you like)
*	form the key. For example, the key for a Socket would be
*	an IP Address and a Port.
*
*	Managers may chain off of anything. They only have a destruction
*	event. This manager takes an engine. Notice engine.event is passed
*	through. The actual base Manager class wants some sort of set of
*	events defining a behavior. Any set will work!
*/
class Manager1 : public Manager<int, int>
{
	public:

		Manager1(Engine& engine) : Manager(engine.event)
		{
			cout << "Created Manager 1" << endl;
			(*this)(1);
		}
		virtual ~Manager1()
		{
			destroy();
			cout << "Destroyed Manager 1" << endl;
		}

}; //Manager1

/*
*	A system is the lowest level object, right next to the engine.
*	They must take an engine. They do not register to the engine, so
*	don't lose track of them. The system has a full pulse behavior, which
*	is tick, destroy, start, and stop.
*
*	The system's job is to tick. Tick is predefined, so if you don't need
*	custom behavior, you don't need to define it. This system uses a custom
*	tick for no reason, lol...
*/
class System1 : public System
{
	private:

		int count;

	public:

		Event<void> eventCustomTick;

		System1(Engine& engine) :
			System(engine),
			count(0)
		{
			cout << "Created System 1" << endl;
		} //System1

		virtual ~System1()
		{
			destroy();

			cout << "Destroyed System 1" << endl;
		}

	protected:

		virtual void onStart()
		{
			cout << "Started System 1" << endl;

			System::onStart();
		} //onStart

		virtual void onStop()
		{
			System::onStop();

			cout << "Stopped System 1" << endl;
		} //onStop

		virtual void onTick(const uint64_t& delta)
		{
			cout << "Ticked System 1(" << delta << ")" << endl;

			if (++count == 3)
			{
				engine.stop();
			} //if
			else
			{
				eventCustomTick.exec();
			} //else
		} //onTick

}; //System1

class System3 : public System
{
	public:

		System3(Engine& engine) :
			System(engine)
		{
			cout << "Created System 3" << endl;
		} //System3

		virtual ~System3()
		{
			destroy();

			cout << "Destroyed System 3" << endl;
		} //~System3

	protected:

		virtual void onStart()
		{
			cout << "Started System 3" << endl;

			System::onStart();
		} //onStart

		virtual void onStop()
		{
			System::onStop();

			cout << "Stopped System 3" << endl;
		} //onStop

		virtual void onTick(const uint64_t& delta)
		{
			cout << "Ticked System 3" << endl;
			
			System::onTick(delta);
		} //onTick

}; //System1

/*
*	An entity manager manages entities. It assigns ids
*	and controls the context of components that entities can have.
*	A component is a property that can be used to expand an entity.
*	For example, an entity might be expanded to have network capabilities.
*	Components tend to work with very specific systems, managers, and entities.
*	An entity is simply defined as something with locality. A system does not have locality, it
*	just floats about. An Actor, an entity belonging to a scene, does have locality. It is local
*	to a scene.
*
*	Scenes and Windows are also entities! They have their own contexts (entity managers) and forward
*	events to other entities.
*
*	A context is a set of mapping that map indices to arrays to behaviors.
*/
class EntityManager1 : public EntityManager
{
	public:

		EntityManager1(Engine& engine, System1& system1, System3& system3) : EntityManager(engine.event)
		{
			context.add(system1);
			context.add(system3);

			cout << "Created Entity Manager 1" << endl;
		} //Manager4

		virtual ~EntityManager1()
		{
			destroy();

			cout << "Destroyed Entity Manager 1" << endl;
		}

		virtual void onStart()
		{
			cout << "Started Entity Manager 1" << endl;

			EntityManager::onStart();
		} //onStart

		virtual void onStop()
		{
			EntityManager::onStop();

			cout << "Stopped Entity Manager 1" << endl;
		} //onStop

}; //EntityManager1

/*
*	Just another system
*/
class System2 : public System
{
	public:

		System1& system1;

		System2(Engine& engine, System1& system1) :
			System(engine),
			system1(system1)
		{
			cout << "Created System 1" << endl;
		}

		virtual ~System2()
		{
			destroy();

			cout << "Destroyed System 2" << endl;
		}

	protected:

		virtual void onStart()
		{
			cout << "Started System 2" << endl;

			System::onStart();
		} //onStart

		virtual void onStop()
		{
			System::onStop();

			cout << "Stopped System 2" << endl;
		} //onStop

		virtual void onTick(const uint64_t& delta)
		{
			cout << "Ticked System 2(" << delta << ")" << endl;

			if (system1.isRunning())
			{
				system1.stop();
			} //if
			else
			{
				system1.start();
			} //else

			System::onTick(delta);
		} //onTick

}; //System2

/*
*	this acts as a point for entities to exist, similarly to a scene
*	it is known that this is a root entity because it goes straight off of
*	the entity manager
*/
class Entity1 : public Entity
{
	public:
		Entity1(EntityManager1& entityManager1, const string name) :
			Entity(entityManager1, name)
		{
			cout << "Created " << name << endl;
		} //Entity1

		virtual ~Entity1()
		{
			destroy();

			cout << "Destroyed " << name << endl;
		} //~Entity1
}; //Entity1

/*
*	this is an entity existing in the above entity
*/
class Entity2 : public Entity
{
	public:
		Entity2(Entity1& entity1) :
			Entity(entity1, entity1.getManager(), "Entity2")
		{
			cout << "Created Entity 2" << endl;
		} //Entity1

		virtual ~Entity2()
		{
			destroy(); cout << "Destroyed Entity 2" << endl;
		} //~Entity2
}; //Entity2

/*
*	this component acts as a gateway to System1 for all entities within
*	Entity1
*/
class Component1 : public Component
{
	private:

		Event<void>::id actionCustomTick;

	protected:

		virtual void onChangeController()
		{
		} //onChangeController

		virtual void onStart()
		{
			cout << "Started " << name << endl;

			actionCustomTick->enable = true;

			Component::onStart();
		} //onStart

		virtual void onStop()
		{
			actionCustomTick->enable = false;

			Component::onStop();

			cout << "Stopped " << name << endl;
		} //onStop

		virtual void onCustomTick()
		{
			/*
			if (name == string("Component 1"))
			{
				stop();
			} //if
			else
			{
				cout << "    Ticked " << name << endl;

				eventCustomTick.exec();
			} //else
			*/

			cout << "    Ticked " << name << endl;

			eventCustomTick.exec();
		} //onCustomTick

	public:

		Event<void> eventCustomTick;

		Component1(Entity1& controller, const string name) :
			Component(controller, controller.getManager().context[typeid(System1)], typeid(System1), name),
			actionCustomTick(((System1&)(System&)(mapping)).eventCustomTick.reg(&Component1::onCustomTick, this))
		{
			cout << "Created " << name << endl;
		} //Component1

		virtual ~Component1()
		{
			actionCustomTick->unreg();

			destroy();

			cout << "Destroyed " << name << endl;
		} //~Component1
}; //Component1

/*
*	This is a component for Entity2 that runs off of Component1 to communicate with
*	System1
*/
class Component2 : public Component
{
	private:

		Event<void>::id actionCustomTick;

	protected:

		virtual void onChangeController()
		{
			Component1& component = ((Component1&)((Entity&)(getController().getController())).components[typeid(System1)]);

			cout << "changed controller to " << component.name << endl;

			setController(component);

			actionCustomTick->unreg();
			actionCustomTick = component.eventCustomTick.reg(&Component2::onCustomTick, this);
		} //onChangeController

		virtual void onStart()
		{
			cout << "Started Component 2" << endl;

			actionCustomTick->enable = true;

			Component::onStart();
		} //onStart

		virtual void onStop()
		{
			actionCustomTick->enable = false;

			Component::onStop();

			cout << "Stopped Component 2" << endl;
		} //onStop

		virtual void onCustomTick()
		{
			cout << "        Ticked Component 2" << endl;

			eventCustomTick.exec();
		} //onCustomTick

	public:

		Event<void> eventCustomTick;

		Component2(Entity2& controller) :
			Component(controller, ((Entity1&)controller.getController()).components[typeid(System1)], typeid(System1), "Component2"),
			actionCustomTick(((Component1&)((Entity1&)controller.getController()).components[mapping]).eventCustomTick.reg(&Component2::onCustomTick, this))
		{
			cout << "Created Component 2" << endl;
		} //Component2

		virtual ~Component2()
		{
			actionCustomTick->unreg();

			destroy();

			cout << "Destroyed Component 2" << endl;
		} //~Component2
}; //Component2

class Component3 : public Component
{
	protected:

		virtual void onChangeController()
		{
		} //onChangeController

		virtual void onStart()
		{
			cout << "Started Component 3" << endl;

			Component::onStart();
		} //onStart

		virtual void onStop()
		{
			Component::onStop();

			cout << "Stopped Component 3" << endl;
		} //onStop

		virtual void onTick(const uint64_t& delta)
		{
			cout << "        Ticked Component 3" << endl;

			Component::onTick(delta);
		} //onCustomTick

	public:

		Component3(Entity2& controller) :
			Component(controller, controller.getManager().context[typeid(System3)], typeid(System3), "Component3")
		{
			cout << "Created Component 3" << endl;
		} //Component2

		virtual ~Component3()
		{
			destroy();

			cout << "Destroyed Component 3" << endl;
		} //~Component2
}; //Component2

/*
*	This is an engine. The variables aren't really needed. Just an example.
*/
class DummyEngine : public Engine
{
	public:

		DummyEngine() :
			Engine()
		{
			auto& system1 = *new System1(*this);
			auto& system2 = *new System2(*this, system1);
			auto& system3 = *new System3(*this);
			
			auto& manager1 = *new Manager1(*this);
			
			auto& entityManager1 = *new EntityManager1(*this, system1, system3);
			
			auto& entity1 = *new Entity1(entityManager1, "Entity 1");
			auto& entity1b = *new Entity1(entityManager1, "Entity 1b");
			auto& entity2 = *new Entity2(entity1);
			
			auto& component1 = *new Component1(entity1, "Component 1");
			auto& component1b = *new Component1(entity1b, "Component 1b");
			auto& component2 = *new Component2(entity2);
			auto& component3 = *new Component3(entity2);
			
			cout << &system1 << &system2 << &system3 << &manager1 << &entityManager1 << &entity1 << &entity1b << &entity2 << &component1 << &component1b << &component2 << &component3 << endl;
			
		} //DummyEngine

}; //DummyEngine

/*
*	Main should ALWAYS look like this!
*/
int main(int argv, char** argc)
{
	DummyEngine engine;
	engine.start();

	return 0;
} //main
