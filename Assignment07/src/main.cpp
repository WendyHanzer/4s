#include <iostream>

#include "Engine/Engine.h"
#include "Engine/Managers/Manager.h"
#include "Engine/Managers/AspectManager.h"
#include "Engine/Managers/AspectSourceManager.h"

#include "Engine/Types/ActiveType.h"
#include "Engine/Types/DestructableType.h"
#include "Engine/Types/ControllerType.h"
#include "Engine/Types/PulseType.h"

#include "Engine/Aspects/AspectSource.h"
#include "Engine/Aspects/Aspect.h"

using namespace std;

class Manager1 : public Manager, public DestructableType
{
	public:
	
		Manager1(Engine& engine) : Manager(engine, typeid(Manager1)), DestructableType(engine.eventDestroy) { }
		virtual ~Manager1() { cout << "Destroyed Manager 1" << endl; }
		
}; //Manager1

class Manager3 : public AspectManager, public PulseType
{
	public:
	
		int count;
		
		//	drop delta
		Event<void> customTick;
	
		Manager3(Engine& engine) :
			AspectManager(engine, typeid(Manager3)),
			PulseType(engine.eventDestroy, engine.eventStart, engine.eventStop, engine.eventTick)
		{ count = 0; }
		
		virtual ~Manager3() { cout << "Destroyed Manager 3" << endl; }
		
		virtual void onStart()
		{
			cout << "Started Manager 3" << endl;
			
			PulseType::onStart();
		} //onStart
		
		virtual void onStop()
		{
			cout << "Stopped Manager 3" << endl;
			
			PulseType::onStop();
		} //onStop
		
		virtual void onTick(const uint64_t& delta)
		{
			cout << "Ticked Manager 3(" << delta << ")" << endl;
			
			if (++count == 3)
			{
				getEngine().stop();
			} //if
			else
			{
				customTick.exec();
			} //else
		} //onTick
		
}; //Manager3

class Manager4 : public AspectSourceManager, public ControllerType
{
	public:
	
		Manager4(Engine& engine) : AspectSourceManager(engine, typeid(Manager4)), ControllerType(engine.eventDestroy, engine.eventStart, engine.eventStop)
		{
			aspectTypes.create(typeid(Manager3));
		} //Manager4
		
		virtual ~Manager4() { cout << "Destroyed Manager 4" << endl; }
		
		virtual void onStart()
		{
			cout << "Started Manager 4" << endl;
			
			ControllerType::onStart();
		} //onStart
		
		virtual void onStop()
		{
			cout << "Stopped Manager 4" << endl;
			
			ControllerType::onStop();
		} //onStop
		
}; //Manager4

class Manager5 : public AspectManager, public PulseType
{
	public:
	
		Manager3& manager3;
		
		//	drop delta
		Event<void> customTick;
	
		Manager5(Engine& engine) :
			AspectManager(engine, typeid(Manager5)),
			PulseType(engine.eventDestroy, engine.eventStart, engine.eventStop, engine.eventTick),
			manager3((Manager3&)engine.managers[typeid(Manager3)])
		{ }
		
		virtual ~Manager5() { cout << "Destroyed Manager 5" << endl; }
		
		virtual void onStart()
		{
			cout << "Started Manager 5" << endl;
			
			PulseType::onStart();
		} //onStart
		
		virtual void onStop()
		{
			cout << "Stopped Manager 5" << endl;
			
			PulseType::onStop();
		} //onStop
		
		virtual void onTick(const uint64_t& delta)
		{
			cout << "Ticked Manager 5(" << delta << ")" << endl;
			
			if (manager3.isActive())
			{
				manager3.stop();
			} //if
			else
			{
				manager3.start();
			} //else
		
			PulseType::onTick(delta);
		} //onTick
		
}; //Manager3

class Source1 : public AspectSource
{
	public:
	
		Source1(Engine& engine) :
			AspectSource((Manager4&)engine.managers[typeid(Manager4)])
		{
			cout << "Created Source 1" << endl;
		} //Source1
		
		virtual ~Source1()
		{
			cout << "Destroyed Source 1" << endl;
		} //~Source1
}; //Source1

class Aspect1 : public Aspect
{
	private:
	
		Event<void>::id onTick;

	public:
	
		Aspect1(Source1& source, Engine& engine) :
			Aspect(source, ((Manager4&)engine.managers[typeid(Manager4)]).aspectTypes[typeid(Manager3)],  ((Manager3&)engine.managers[typeid(Manager3)])),
			onTick(getController<Manager3>().customTick.reg(&Aspect1::customTick, this))
		{
			cout << "Created Aspect 1" << endl;
		} //Aspect1
		
		virtual ~Aspect1()
		{
			cout << "Destroyed Aspect 1" << endl;
		} //~Aspect1
		
		virtual void onStart()
		{
			cout << "Started Aspect 1" << endl;
			
			Aspect::onStart();
		} //onStart
		
		virtual void onStop()
		{
			cout << "Stopped Aspect 1" << endl;
			
			Aspect::onStop();
		} //onStop
		
		void customTick()
		{
			cout << "Ticked Aspect 1" << endl;
		} //onTick
}; //Aspect

class DummyEngine : public Engine
{
	public:
	
		virtual void init()
		{
			new Manager1(*this);
			new Manager3(*this);
			new Manager4(*this);
			new Manager5(*this);
			
			auto s1 = new Source1(*this);
			
			new Aspect1(*s1, *this);
		} //init
}; //DummyEngine

int main(int argv, char** argc)
{
	DummyEngine engine;
	engine.init();
	engine.start();

	return 0;
} //main
