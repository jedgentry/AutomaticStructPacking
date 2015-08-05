#include "TokenMethods.h"

TokenMethods::TokenMethods::TokenMethods()
{
	myStructParser = new StructContainer::StructContainer();
	myStructParser->init();
}

TokenMethods::TokenMethods::~TokenMethods()
{
	delete myStructParser;
}

//This means that the struct keyword has been found.
void TokenMethods::TokenMethods::tokenStruct(const DoublyLinkedList::Node<std::string>* current)
{
	processStructs((DoublyLinkedList::Node<std::string>*) current);
}

void TokenMethods::TokenMethods::processStructs(DoublyLinkedList::Node<std::string>* current)
{
	myStructParser->addStruct(current);
	myStructParser->packStructs();
	//TODO: Edit them back into the list.
}

void TokenMethods::TokenMethods::tokenStructResult(const DoublyLinkedList::Node<std::string>* current)
{
	myStructParser->insertResults((DoublyLinkedList::Node<std::string>*)current);
}