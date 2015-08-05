#include "StructContainer.h"
/* Debug quicksorted list. */
StructContainer::StructContainer::StructContainer()
{
	structMetaData.reserve(INITIAL_STRUCT_RESERVE);
	inputFile.open(KNOWN_TYPES_LIST);
	if (!inputFile.is_open())
	{
		std::cerr << "Unable to load data types!\n Looking for file: " << KNOWN_TYPES_LIST << std::endl;
		exit(-1);
	}
	elementSwapBuffer = new DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>;
	padding = true;
}

StructContainer::StructContainer::~StructContainer()
{
	clearStructMeta();
	delete elementSwapBuffer;
}

void StructContainer::StructContainer::init()
{
	loadKnownTypes();
	//Best match list.
	candidates = new DoublyLinkedList::DoublyLinkedList < SharedStructDefinitions::StructElement* >();
	paddingNumber = 0;
}

void StructContainer::StructContainer::addStruct(DoublyLinkedList::Node<std::string>* start)
{
	StructMeta* myMeta = initStructMeta(start);
	findEndOfStructDefinition(start, myMeta);
	//Check for error.
	if (start->data != "struct")
	{
		std::cerr << "Object passed in with data: " << start->data << " was not 'struct'" << std::endl;
		return;
	}
	findTypedefKeyword(start, myMeta);
	findStructName(start, myMeta);
	findStructElements(start, myMeta);
	structMetaData.push_back(myMeta);
}

void StructContainer::StructContainer::insertResults(DoublyLinkedList::Node<std::string>* start)
{
	//Currently at the starting point of the "struct" keyword.
	//Match it to the appropriate struct meta start.
	unsigned int index = 0;
	for (index = 0; index < structMetaData.size(); index++)
	{
		//Go to the correct Struct.
		if (structMetaData[index]->start == start)
			break;
	}
	//Edit linked List.
	insertPackedElementsIntoList(start, structMetaData[index]);
}

/* PADDING SHOULD BE AN OPTION SO MAKE SURE TO INDEX WHERE ITS INSERTED. */
void StructContainer::StructContainer::packStructs()
{
	//Simulate and repack, and add padding.
	for (unsigned int i = 0; i < structMetaData.size(); i++)
	{
		sortAndPackElements(structMetaData[i]);
	}
	//Changes are done and made, spit out data.
}

///////////////////////
// Private Functions //
///////////////////////

void StructContainer::StructContainer::insertPackedElementsIntoList(DoublyLinkedList::Node<std::string>* start,
	StructMeta* myMeta)
{	/*	TODO:
		Keep a current index at the start
		Turn all elements into a series of DoublyLinkedList::Node<std::string> tied together.
		Move the start->next to the start of the elements list tied together.
		Move the end->previous to the end of the list of elements tied together.
		Turn the end->previous->next pointer to nullptr.
		Delete from the current to the end of the moved nodes.
	*/
	DoublyLinkedList::Node<std::string>* current = start, *insertionPoint = start;
	DoublyLinkedList::Node<std::string>* swapNode = createStringListOfStructElements(myMeta);
	unsigned int deletedNodes = 0;
	while (start != nullptr && start->data.rfind("}") == std::string::npos)
	{
		//Delete unnecisary text.
		current = start;
		start = start->next;
		delete current;
		deletedNodes++;
	}
	insertionPoint->next = swapNode;
	//TODO: UPDATE COUNT HERE, Need raw text list to update count.
	start->previous = myMeta->end;
}

DoublyLinkedList::Node<std::string>* StructContainer::StructContainer::createStringListOfStructElements(StructMeta* myMeta)
{
	DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* currentElement = myMeta->packedStructElements->getHead();
	DoublyLinkedList::Node<std::string>* head = nullptr, *currentNode = nullptr;
	unsigned int i = 0;
	while (currentElement != nullptr)
	{
		if (i == 0)
		{
			head = structElementToStringNode(currentElement->data, nullptr, nullptr);
		}
		else if (i == 1)
		{
			currentNode = structElementToStringNode(currentElement->data, head, nullptr);
		}
		else
		{
			currentNode = structElementToStringNode(currentElement->data, currentNode, nullptr);
		}
		//iterate
		i++;
		currentElement = currentElement->next;
	}
	myMeta->count = i;
	myMeta->end = currentNode;
	return head;
}

DoublyLinkedList::Node<std::string>* StructContainer::StructContainer::structElementToStringNode(SharedStructDefinitions::StructElement* myElement)
{
	DoublyLinkedList::Node<std::string>* mNode = new DoublyLinkedList::Node<std::string>;
	mNode->data += myElement->structDataType + myElement->variableName;
	mNode->previous = nullptr;
	mNode->next = nullptr;
	return mNode;
}

DoublyLinkedList::Node<std::string>* StructContainer::StructContainer::structElementToStringNode(SharedStructDefinitions::StructElement* myElement,
	DoublyLinkedList::Node<std::string>* previous, DoublyLinkedList::Node<std::string>* next)
{
	DoublyLinkedList::Node<std::string>* mNode = new DoublyLinkedList::Node<std::string>;
	mNode->data += myElement->structDataType + myElement->variableName;
	mNode->previous = previous;
	mNode->next = next;
	return mNode;
}

void StructContainer::StructContainer::findEndOfStructDefinition(DoublyLinkedList::Node<std::string>* start,
	StructMeta* myMeta)
{
	while (start != nullptr && start->data.rfind("};") == std::string::npos)
	{
		start = start->next;
	}
	myMeta->end = start;
}

void StructContainer::StructContainer::sortAndPackElements(StructMeta* myMeta)
{
	//Sort the elements before packing.
	quicksortElements(myMeta);
	//Begin packing.
	packElements(myMeta);
}

void StructContainer::StructContainer::quicksortElements(StructMeta* myMeta)
{
	quicksortElementList(myMeta->structElements->getHead(), myMeta->structElements->getTail());
}

void StructContainer::StructContainer::findTypedefKeyword(DoublyLinkedList::Node<std::string>* start, StructMeta* myMeta)
{
	if (start->previous != nullptr)
	{
		if (start->previous->data == "typedef")
		{
			myMeta->hasTypedef = true;
		}
	}
}

void StructContainer::StructContainer::findStructElements(DoublyLinkedList::Node<std::string>* start, StructMeta* myMeta)
{
	/* Check if exists, if not, check if pointer, else dont bother.*/
	while (start->data.rfind("}") == std::string::npos)
	{
		try
		{
			if (knownTypes.at(start->data))
			{
				//We have a known size.
				SharedStructDefinitions::StructElement* myElement = initStructElement(start);
				myElement->start = start;
				myElement->size = knownTypes.at(start->data);
				//Update max denominator.
				if (myMeta->maxSize < myElement->size)
					myMeta->maxSize = myElement->size;
				myElement->structDataType = start->data;
				//Get rid of semicolon.
				myElement->variableName = start->next->data.substr(0, start->next->data.length() - 1);
				//TODO ADD NAME
				myMeta->structElements->insertAtTail(myElement);
				//Move forward one more since we dont need the name to be scanned.
				start = start->next;
			}
		}
		catch (const std::out_of_range)
		{ 
			//Do nothing since all we can do is iterate.
		}
		start = start->next;
	}
}

void StructContainer::StructContainer::loadKnownTypes()
{
	std::string buffer = "";
	unsigned int sizeBuffer = 0;
	while (!inputFile.eof())
	{
		std::getline(inputFile, buffer);
		//Set last bit to integer buffer and move the rest
		sizeBuffer = toDigit(buffer[buffer.length() - 1]);
		buffer = buffer.substr(0, buffer.length() - 2);
		//Put in the buffer and the size.
		knownTypes.insert(std::make_pair(buffer, sizeBuffer));
		//Reset variables.
		buffer = "";
		sizeBuffer = 0;
	}
	inputFile.close();
}

void StructContainer::StructContainer::printStructInfo(DoublyLinkedList::Node < std:: string>* start, bool atStart)
{
	if (atStart)
	{
		while (start != nullptr && start->data != ";")
			std::cerr << start->data << std::endl;
	}
	else
	{
		//Rewind.
		while (start != nullptr && start->data != "struct")
			start = start->previous;
		//Print the error
		//SWITCH TO COMPARE AND CHECK FOR NEXT BEING NULLPTR
		while (start != nullptr && (start->data.compare("};") != 0))
		{
			std::cerr << start->data << std::endl;
			start = start->next;
		}
		//Print the last brace
		std::cerr << start->data << std::endl;
	}
}

void StructContainer::StructContainer::findStructName(DoublyLinkedList::Node<std::string>* start, StructMeta* myMeta)
{
	//Read Till bracket and check if its defined the first way.
	while (start->data != "{" && start != nullptr)
		start = start->next;
	//We're now at the bracket.
	if (start->previous->data != "struct" && start->previous != nullptr)
	{
		//Then the name is behind us.
		myMeta->structName = start->previous->data;
		myMeta->nameBefore = true;
	}
	else
	{
		findStructNameAfterDeclaration(start, myMeta);
	}
}

void StructContainer::StructContainer::findStructNameAfterDeclaration(DoublyLinkedList::Node<std::string>* start, StructMeta* myMeta)
{
	//The name is after the closing brace.
	DoublyLinkedList::Node<std::string>* ptr = start;
	while (start->next != nullptr && start->data != "}")
		start = start->next;
	//Double check that it is correct
	if (start != nullptr && start->previous != nullptr && start->previous->data != ";")
	{
		//Assign the name
		std::string buffer = start->previous->data;
		if (buffer[buffer.length() - 1] == ';')
			buffer = buffer.substr(0, buffer.length() - 2);
		myMeta->nameBefore = false;
	}
	else
	{
		//An error occured on acquiring the name.
		std::cerr << "Unable to acquire struct name! Here are the contents: " << std::endl;
		printStructInfo(ptr, false);
	}
}

StructContainer::StructMeta* StructContainer::StructContainer::initStructMeta(DoublyLinkedList::Node<std::string>* start)
{
	StructMeta* myMeta = new StructMeta;
	myMeta->structName = "";
	myMeta->hasTypedef = false;
	myMeta->nameBefore = false;
	myMeta->sorted = false;
	myMeta->maxSize = 0;
	myMeta->structElements = new DoublyLinkedList::DoublyLinkedList<SharedStructDefinitions::StructElement*>();
	myMeta->packedStructElements = new DoublyLinkedList::DoublyLinkedList<SharedStructDefinitions::StructElement*>();
	myMeta->start = start;
	myMeta->end = nullptr;
	myMeta->count = 0;
	return myMeta;
}

SharedStructDefinitions::StructElement* StructContainer::StructContainer::initStructElement(DoublyLinkedList::Node<std::string>* start)
{
	SharedStructDefinitions::StructElement* element = new SharedStructDefinitions::StructElement();
	element->structDataType = "";
	element->size = 0;
	element->start = start;
	element->used = false;
	return element;
}

SharedStructDefinitions::StructElement* StructContainer::StructContainer::createPaddingStruct(unsigned int sectorSize)
{
	//This means we can pad
	SharedStructDefinitions::StructElement* myElement = initStructElement(nullptr);
	myElement->size = sectorSize;
	std::string buffer = "char padding" + std::to_string(paddingNumber) + "[" + std::to_string(sectorSize) + "];\n";
	paddingNumber++;
	myElement->structDataType = "char";
	myElement->used = true;
	return myElement;
}

void StructContainer::StructContainer::clearStructMeta()
{
	//Delete dynamically allocated memory.
	unsigned int size = structMetaData.size();
	for (unsigned int i = 0; i < size; i++)
	{
		structMetaData[i]->structName.erase();
		structMetaData[i]->structElements->~DoublyLinkedList();
		structMetaData[i]->structElements = nullptr;
		structMetaData[i]->start = nullptr;
		delete structMetaData[i];
	}
}

DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* StructContainer::StructContainer::quicksortPivot(DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>*  head,
	DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>*  tail)
{
	unsigned int pivotValue = tail->data->size;
	DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* pivot = tail->previous;
	for (DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* i = head;
	i != tail; i = i->next)
	{
		if (i->data->size < pivotValue)
		{
			if (i == nullptr)
			{
				i = head;
			}
			else
			{
				if (i->next != nullptr)
				{
					i = i->next;
				}
				else
				{
					//Hit the end of the list.
					break;
				}
			}
			quicksortSwap(pivot, i);
		}
	}
	if (pivot == nullptr)
	{
		pivot = head;
	}
	else
	{
		pivot = pivot->next;
	}
	quicksortSwap(pivot, tail);
	return pivot;
}

void StructContainer::StructContainer::quicksortElementList(DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* head,
	DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>*  tail)
{
	if (head != nullptr && tail != nullptr && tail != head)
	{
		DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* pivot = quicksortPivot(head, tail);
		//quicksort here left and right recursivelyx
		quicksortElementList(head, tail->previous);
		quicksortElementList(head->next, tail);
	}
}

void StructContainer::StructContainer::quicksortSwap(DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* swapA,
	DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* swapB)
{
	if (swapA != nullptr && swapB != nullptr)
	{
		elementSwapBuffer->data = swapA->data;
		swapA->data = swapB->data;
		swapB->data = elementSwapBuffer->data;
	}
}

//Goes through each element.
void StructContainer::StructContainer::packElements(StructMeta* myMeta)
{
	DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* current = myMeta->structElements->getHead();
	unsigned int padding = 0;
	//Iterate through all the elements.
	while (current != nullptr)
	{
		//Get current leftover.
		if (current->data->used == false)
		{
			padding = myMeta->maxSize - current->data->size;
			//Add it to the list and mark as used.
			myMeta->packedStructElements->insertAtTail(current->data);
			current->data->used = true;
			if (padding > 0)
			{
				//Find next match - takes the next pointer due to the match already being in packed elements.
				findBestMatch(current->next, padding, myMeta);
			}
		}
		//Iterate.
		padding = 0;
		current = current->next;
	}
	paddingNumber = 0;
}

//Matches by biggest fit first.
void StructContainer::StructContainer::findBestMatch(DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* current,
	unsigned int padding, StructMeta* myMeta)
{
	while (current != nullptr && padding > 0)
	{
		//Add anything thats less than the list to it.
		if (current->data->size <= padding && !current->data->used)
		{
			//Add to the sorted list.
			padding -= current->data->size;
			myMeta->packedStructElements->insertAtTail(current->data);
			current->data->used = true;
		}
		current = current->next;
	}
	definePadding(current, padding, myMeta);
}

//Creates padding - eventually will only need the padding number.
void StructContainer::StructContainer::definePadding(DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* current,
	unsigned int padding, StructMeta* myMeta)
{
	if (padding == 0)
	{
		return;
	}
	else if (padding < 0)
	{
		std::cerr << "Padding was below zero for struct named: " << myMeta->structName << " with element: " <<
			current->data->variableName << " with size: " << current->data->size << std::endl;
		return;
	}
	else
	{
		myMeta->packedStructElements->insertAtTail(createPaddingStruct(padding));
	}
}