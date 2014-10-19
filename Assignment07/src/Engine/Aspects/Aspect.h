#pragma once

#include "AspectSource.h"
#include "../Managers/TypeManager.h"
#include "../Types/PulseType.h"

class AspectSource;

class Aspect
{
	private:

		Event<void>::id onDestroyActionSource;
		Event<void>::id onDestroyActionController;
		Event<void>::id onStartActionSource;
		Event<void>::id onStartActionController;
		Event<void>::id onStopActionSource;
		Event<void>::id onStopActionController;
		Event<const uint64_t&>::id onTickAction;

		TypeManager::Type& type;
		AspectSource& source;
		PulseType& controller;

		bool active;

		void onDestroy();

	protected:

		Aspect(AspectSource& source, TypeManager::Type& type, PulseType& controller);

		virtual void onStart();
		virtual void onStop();
		virtual void onTick(const uint64_t& delta) { };

	public:

		virtual ~Aspect();

		void start();
		void stop();

		inline TypeManager::Type& getType() { return type; }
		inline const bool& isActive() { return active; }
		
		template <class T>
		inline T& getSource() { return dynamic_cast<T&>(source); }
		
		template <class T>
		inline T& getController() { return dynamic_cast<T&>(controller); }

}; //Aspect
