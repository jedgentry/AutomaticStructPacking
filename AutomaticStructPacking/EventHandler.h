#ifndef EventHandler_H
#define EventHandler_H

#include <functional>
#include <map>
#include <vector>
#include <utility> //for std::Forward
#include <iostream> //for output.
#include <exception>

#include "DoublyLinkedList.h"
#include "SharedStructDefinitions.h"

/* This class could potentially handle error checking for 
finding if an event is not in the list, but need to find out
if maps search through a bst or linearly, cause current bst 
implementation is faster.								   */

namespace EventHandler
{
	//The typename for the events that are called here.
	template <typename Event>
	//Handles events that are triggered using callbacks.
	class EventHandler
	{
	public:
		//Default Constructor
		EventHandler() = default;
		//Registers an Observer to the list.
		//@param Event& - The event to add to the trigger list.
		//@param Observer&& - The function/observer that will be called.
		template <typename Observer>
		void registerObserver(const Event& event, Observer&& observer)
		{
			observers[event].push_back(std::forward<Observer>(observer));
			std::cout << "Registered an observer using &..." << std::endl;
		}
		//Registers an Observer to the list. Move Constructor.
		//@param Event& - The event to add to the trigger list.
		//@param Observer&& - The function/observer that will be called.
		template <typename Observer>
		void registerObserver(Event&& event, Observer&& observer)
		{
			observersWithNode[std::move(event)].push_back(std::forward<Observer>(observer));
			std::cout << "Registered an observer using &&..." << std::endl;
		}
		//Registers to call an observer with results of the file.
		//@param Event&& event - the event to add to the trigger list.
		//@param Observer&& - the function/observer that will be called.
		template <typename Observer>
		void registerObserverResults(const Event& event, Observer&& observer)
		{
			observersWithNodeResults[event].push_back(std::forward<Observer>(observer));
			std::cout << "Registered an observer for node results &..." << std::endl;
		}
		//Registers to call an observer with results of the file.
		//@param Event&& event - the event to add to the trigger list.
		//@param Observer&& - the function/observer that will be called.
		template <typename Observer>
		void registerObserverResults(const Event&& event, Observer&& observer)
		{
			observersWithNodeResults[std::move(event)].push_back(std::forward<Observer>(observer));
			std::cout << "Registered an observer for node results &&..." << std::endl;
		}
		//Notifies the observers listening for this event.
		//@param Event& the event that will trigger the observers.
		//@param DoublyLinkedList::Node<std::string>* - pointer to the node at trigger event.
		void notifyWithNode(const Event& event, DoublyLinkedList::Node<std::string>* node) const
		{
			try
			{
				for (auto& observersWithNodeIter : observersWithNode.at(event))
				{
					observersWithNodeIter(node);
				}
			}
			catch (const std::out_of_range)
			{
				//TODO: Why is this still caused?
				//Do nothing.
			}
		}
		//Notifies the observers listening for this event.
		//@param Event& the event that will trigger the observers.
		//@param DoublyLinkedList::Node<std::string>* - pointer to the node at trigger event.
		void notifyResultsWithNode(const Event& event, DoublyLinkedList::Node<std::string>* node) const
		{
			try
			{
				for (auto& observersWithNodeResultsIter : observersWithNodeResults.at(event))
				{
					observersWithNodeResultsIter(node);
				}
			}
			catch (const std::out_of_range)
			{
				//Do nothing.
			}
		}
		//Notifies the observers listening for a type event.
		//@param Event& the even that will trigger the observers.
		//@param StructElement - the fields to fill out from the events.
		void notify(const Event& event, SharedStructDefinitions::StructElement* myElement) const
		{
			for (auto& observerWithElementsIter : observersWithNode.at(event))
			{
				observersWithElementsIter(myElement);
			}
		}
		
		//Disable copying and assigning
		EventHandler(const EventHandler&) = delete;
		EventHandler& operator=(const EventHandler&) = delete;
	private:
		//Map of functions containing events that need a Node* trigger.
		std::map<Event, std::vector<std::function<void(const DoublyLinkedList::Node<std::string>* node)>>> observersWithNode;
		//Map of functions containing events that need a SharedStructDefinitions::StructElement* trigger.
		std::map<Event, std::vector<std::function<void(SharedStructDefinitions::StructElement* myElement)>>> observersWithElements;
		//Map of the functions containing the results of their events.
		std::map<Event, std::vector<std::function<void(const DoublyLinkedList::Node<std::string>* node)>>> observersWithNodeResults;
	};
}
#endif