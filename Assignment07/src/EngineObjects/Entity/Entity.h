#pragma once

#include "../MoveableObject.h"

#include "Aspects/AspectType.h"
#include "Aspects/Aspect.h"

#include "../../Managers/EntityManager.h"

class EntityManager;

class Entity : public MoveableObject { /*
*
*/	private: /*
*
*		Fields
*		----------------------------------
*/
/*			static int idCount;
			static stack<int> idRecycler;

			static int allocateId();
			static void deallocateId(int id);

			static map<int, Entity*> entityTable;
*/ // Moved to EntityManager and made non-static
			int id;
/*
*****************************************************************
*
*/	public: /*
*
*		Constructors
*		----------------------------
*
*/			Entity(); /*
*			-	Constructs an entity
*
*/			~Entity(); /*
*			-	Destroys an entity
*
*		Methods??
*		----------------------------------
*
*/
			Event<double> eventTick;
			Event<Entity*> eventDestroy;

			//this can be an array instead
			//int is the aspectTypeId, which is assigned to
			//each aspectType
			//the total size of the array is the total number
			//of aspect types, which is part of the AspectType class
			//currently this stuff isn't in, so it needs to be added

			//change the following to an array
			//map<AspectType*, Aspect*> aspects;

			inline int getId() { return id; }

/*
*
*
*****************************************************************/
};