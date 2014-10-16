#pragma once

/*
*	Event
*
*		Event()
*		~Event()
*		//	don't destroy an event while executing it
*
*		void clear()
*		void reverse()
*
*		Event::id reg(function, args...)

*		Event::id->enable = true|false;
*		Event::id->unreg();
*		Event::id->replace(function, args...)
*
*		void exec()
*			-	runs all functions on Event
*/

#include <atomic>

#include "EasyBind.h"

template<typename... Params>
class Event
{
    /*
    *    Structures
    */
    struct Node
    {
        std::function<void(Params...)> dat;
        bool enable;
        bool destroy;
		Node* next;
		Node* prev;
		Event* event;

		inline void unreg()
		{
			destroy = true;
		} //unreg

		template<class T, class... Args>
		void replace(T&& func, Args&&... args)
        {
            dat = easy_bindEx<T, Params..., Args...>(func, args...);
        } //replace
	}; //Node

	/*
	*	Fields
	*/
	Node* first;
	Node* last;

	bool reversed;

	void unreg(id eventId)
	{
		if (eventId->next)
		{
			eventId->next->prev = eventId->prev;
		} //if
		else
		{
			last = eventId->prev;
		} //else
		if (eventId->prev)
		{
			eventId->prev->next = eventId->next;
		}
		else
		{
			first = eventId->next;
		} //else
	} //unreg

public:
	/*
	*	Types
	*/
	typedef Node* id;

	/*
	*	Constructors
	*/
	Event()
	{
		first = NULL;
		last = NULL;

		reversed = false;
	} //Event

	~Event()
	{
		Node* next;

		while (first)
		{
			next = first->next;
			delete first;
			first = next;
		} //while

		last = NULL;
	} //~Event

	/*
	*	Data Manipulation Operations
	*/
	template<class T, class... Args>
	id reg(T&& func, Args&&... args)
	{
		Node* node = new Node();
		node->dat = easy_bindEx<T, Params..., Args...>(func, args...);
		node->event = this;
		node->enable = true;
		node->destroy = false;

		if (reversed)
		{
			node->prev = NULL;
			node->next = first;

			if (first == NULL)
			{
				last = node;
			} //if
			else
			{
				first->prev = node;
			} //else

			first = node;
		} //if
		else
		{
			node->next = NULL;
			node->prev = last;

			if (last == NULL)
			{
				first = node;
			} //if
			else
			{
				last->next = node;
			} //else

			last = node;
		} //else

		return node;
	} //reg

	/*
    template<class... P, class... Args>
    id regEvent(Event<P...>& event, Args&&... args)
    {
        easy_bind(std::function<void(Event<P...>&, P...)>([](Event<P...>& event, Args&&... args, Params&&... params) { event.exec(forward<Args>(args)..., forward<Params>(params)...);}), event, args...);
        return NULL;
        //return reg(easy_bind(std::function<void(Event<P...>&, P...)>([](Event<P...>& event, Args&&... args, Params&&... params) { event.exec(forward<Args>(args)..., forward<Params>(params)...);}), event, args...));
    } //regEvent
    */

	void clear()
	{
		for (Node* node = first; node; node = node->next)
		{
			node->unreg();
		} //for
	} //clear

	/*
	*	Structure Manipulation Operations
	*
	*/
	void reverse()
	{
		if (first)
		{
			Node* node = first;
			Node* next = node->next;

			while (next)
			{
				node->next = node->prev;
				node->prev = next;
				node = next;
				next = node->next;
			} //while

			last->next = last->prev;
			last->prev = next;

			node = last;
			last = first;
			first = node;

			reversed = !reversed;
		} //if
	} //reverse

	/*
	*	Execution Operations
	*/
	void exec(Params&&... args)
	{
		Node* node = first;
		Node* next;

		while (node)
		{
			next = node->next;

			if (node->destroy)
			{
				unreg(node);
				delete node;
			} //if
			else if (node->enable)
			{
				node->dat(args...);
			} //else if

			node = next;
		} //while
	} //exec
}; //Event