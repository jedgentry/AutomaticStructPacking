#ifndef SHAREDSTRUCTDEFINITIONS_H
#define SHAREDSTRUCTDEFINITIONS_H

/* SHARED DEFINITIONS */
namespace SharedStructDefinitions
{
	//The elements in the struct.
	struct StructElement
	{
		//The data type that was in this struct.
		std::string structDataType;
		//The name of the variable.
		std::string variableName;
		//The start of where this data type was.
		DoublyLinkedList::Node<std::string>* start;
		//The size this took in memory.
		unsigned int size;
		//If this struct has been used or not
		bool used;
	};
	//TODO REMOVE ONE OF THESE.
	//The data on a supported element
	struct ElementData
	{
		//name of the element.
		std::string name;
		//Size of the element.
		unsigned int size;
	};
}

#endif