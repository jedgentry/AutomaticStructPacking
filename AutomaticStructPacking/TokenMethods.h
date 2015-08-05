#ifndef TOKENMETHODS_H
#define TOKENMETHODS_H

#include "DoublyLinkedList.h"
#include "StructContainer.h"

namespace TokenMethods
{
	class TokenMethods
	{
	public:
		//Default Constructor
		TokenMethods();
		//Default Destructor
		~TokenMethods();
		//Methods for stumbling upon struct keyword
		void tokenStruct(const DoublyLinkedList::Node<std::string>* current);
		//Results for said message
		void tokenStructResult(const DoublyLinkedList::Node<std::string>* current);
		//General Process function for structs
		void processStructs(DoublyLinkedList::Node<std::string>* current);
	private:
		//Internal states of computing the tokens.
		StructContainer::StructContainer* myStructParser;
	};
}

#endif