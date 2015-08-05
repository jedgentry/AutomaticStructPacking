#ifndef STRUCTCONTAINER_H
#define STRUCTCONTAINER_H

#include <iostream> //cerr
#include <string>
#include <vector>
#include <fstream> //Read in known types
#include <cctype> //isdigit
#include <exception>

#include "DoublyLinkedList.h"
#include "BinarySearchTree.h"
#include "EventHandler.h"
#include "SharedStructDefinitions.h"

/* TODO: TEACH IT TO ASSIGN POINTERS */
/*
	TODO: IN ORDER.
	1) Parse structs and fill out field elements, and their sizes.
	2) Organize by size allocation.
	3) Add padding if there is any.
	4) Spit out newly optimized struct, along with how much memory it saves per struct.
	5) Edit into new file??
*/
namespace StructContainer
{
	//Initial amount of memory to reserve.
	const unsigned int INITIAL_STRUCT_RESERVE = 64;
	//The filename to load types from.
	const std::string KNOWN_TYPES_LIST = "data_types.txt";
	//Defined pointer types of 32 bit and 64bit system
	const unsigned int X32_BIT_POINTER_LENGTH = 4;
	const unsigned int X64_BIT_POINTER_LENGTH = 8;

	//The meta information on the struct
	struct StructMeta
	{
		//Name of the struct
		std::string structName;
		//A list of the elements that belonged in this struct.
		DoublyLinkedList::DoublyLinkedList<SharedStructDefinitions::StructElement*>* structElements;
		//The organized list.
		DoublyLinkedList::DoublyLinkedList<SharedStructDefinitions::StructElement*>* packedStructElements;
		//The start of the raw text that originally defined this struct.
		DoublyLinkedList::Node<std::string>* start;
		//The end of the raw text that is now defining the struct.
		DoublyLinkedList::Node<std::string>* end;
		//Count of the final elements.
		unsigned int count;
		//This is the maximum size denominator of the struct.
		unsigned int maxSize;
		//If this has been sorted or not.
		bool sorted;
		//If there was a typedef for this struct
		bool hasTypedef;
		//Where the name was located for this struct
		bool nameBefore;
	};

	//Class containing all the structs.
	class StructContainer
	{
	public:
		//Deafult constructor.
		StructContainer();
		//Deafult Destructor.
		~StructContainer();
		//Initializes the class.
		void init();
		//Adds a struct to the list and fills out its parameters through parsing.
		void addStruct(DoublyLinkedList::Node<std::string>* start);
		//Inserts the results back into the chain.
		void insertResults(DoublyLinkedList::Node<std::string>* start);
		//Reogranizes the structs in memory
		void packStructs();
		//Sets
		void setPaddingSetting(bool padding) { this->padding = padding; };
		//Gets
		bool getPaddingSetting() { return padding; };
	private:
		//Sorts and packs the elements.
		//@param StructMeta* - The meta to sort and pack.
		void sortAndPackElements(StructMeta* myMeta);
		//Packing algorithm
		//@param StructMeta* the meta to pack.
		void packElements(StructMeta* myMeta);
		//Quicksorts the elements in the list.
		//@param StructMeta* - the elements to sort.
		void quicksortElements(StructMeta* myMeta);
		//Quicksort Helper function for swapping elements.
		//@param DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* first element to swap.
		//@param DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* second element to swap.
		void quicksortSwap(DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* swapA,
			DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* swapB);
		//Creates a pivot for quicksort
		//@param DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* head - head to start at.
		//@param DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* tail - tail to end pivot width.
		//@return DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* the result of the pivot.
		DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* quicksortPivot(
			DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>*  head,
			DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>*  tail);
		//Quicksort for elements
		//@param DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* the head to start the unsorted list.
		//@param DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* the tail end of the unsorted list.
		void quicksortElementList(DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>*  head,
			DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>*  tail);
		//Loads the known types from the file into the bst.
		void loadKnownTypes();
		//Inits the struct meta in memory.
		//@param Node<std::string>* - the starting point of the prase list.
		//@return StructMeta - the newly instansiated object.
		StructMeta* initStructMeta(DoublyLinkedList::Node<std::string>* start);
		//Inits the struct in memory
		//@param start - the start of the text to parse.
		//@return StructElement* - a pointer to the newly intialized struct.
		SharedStructDefinitions::StructElement* initStructElement(DoublyLinkedList::Node<std::string>* start);
		//Finds the name of the struct in memory.
		//@param Node<std::string>* - the place to start looking for the name of the structure.
		//@param StructMeta* - the StructMeta to add this information to.
		void findStructName(DoublyLinkedList::Node<std::string>* start, StructMeta* myMeta);
		//Finds the name of the struct if it comes after the brace.
		//@param Node<std::string>* - the place to start looking for the name.
		//@param StructMeta* - Where to add the found name to.
		void findStructNameAfterDeclaration(DoublyLinkedList::Node<std::string>* start, StructMeta* myMeta);
		//Finds the elements for a particular struct and adds them to the list.
		//@param Node<std::string>* - the place to start looking for elements.
		//@param StructMeta* - where to add the list of elements to.
		void findStructElements(DoublyLinkedList::Node<std::string>* start, StructMeta* myMeta);
		//Prints the structs information from memory.
		//@param Node<std::string>* - the starting point to print from.
		//@param bool - If this is true, it will print forwards.
		//otherwise it will rewind till it hits 'struct' and print.
		void printStructInfo(DoublyLinkedList::Node<std::string>* start, bool atStart);
		//Finds if theres a typdef keyword for this struct.
		//@param Node<std::string>* - the starting point.
		//@param StructMeta* - the meta that we will be adding the data to.
		void findTypedefKeyword(DoublyLinkedList::Node<std::string>* start, StructMeta* myMeta);
		//Clears the datatype list of dynamically allocated memory.
		void clearStructMeta();
		//Finds the best match of padding for the struct, and moves the elemnts to packed list.
		//@param DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* the current item to match for.
		//@param unsigned int the amount of padding left to fill with another item.
		//@param StructMeta* the struct in memory that we're sorting.
		void findBestMatch(DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* current,
			unsigned int padding, StructMeta* myMeta);
		//Creates a struct of padding with size sectorSize
		//@param unsigned int - the size of the padding to create.
		//@return SharedStructDefinition::StructElement* - struct with padding.
		SharedStructDefinitions::StructElement* createPaddingStruct(unsigned int sectorSize);
		//The locations of structs parsed into our memory pattern.
		std::vector<StructMeta*> structMetaData;
		//Defines padding for the user.
		//@param DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* current - place to insert padding.
		//@param unsigned int - the amount of padding to generate.
		//@param StructMeta* - the structmeta to insert the resulting padding into.
		void definePadding(DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* current,
			unsigned int padding, StructMeta* myMeta);
		//Inserts the packed struct elements into the list.
		void insertPackedElementsIntoList(DoublyLinkedList::Node<std::string>* start, StructMeta* myMeta);
		//Turns a struct element into a string.
		DoublyLinkedList::Node<std::string>* structElementToStringNode(SharedStructDefinitions::StructElement* myElement);
		//Overloaded, sets additional pointers.
		DoublyLinkedList::Node<std::string>* structElementToStringNode(SharedStructDefinitions::StructElement* myElement,
			DoublyLinkedList::Node<std::string>* previous, DoublyLinkedList::Node<std::string>* next);
		//Creates a list of elements and turns them into a list of DoublyLinkedList::Node<std::string> tied togehter.
		DoublyLinkedList::Node<std::string>* createStringListOfStructElements(StructMeta* myMeta);
		//The tree that holds the types we are capable of processing.
		std::map<std::string, unsigned int> knownTypes;
		//File stream to load in the types.
		std::ifstream inputFile;
		//Quick and dirty to digit.
		//@param char - what to turn into a digit.
		//@return unsigned int - the character in integer form.
		inline unsigned int toDigit(char ch) { return ch - '0'; };
		//Finds the end of the struct definition.
		void findEndOfStructDefinition(DoublyLinkedList::Node<std::string>* start, StructMeta* myMeta);
		//Holds the list of best candidates.
		//Best match list.
		DoublyLinkedList::DoublyLinkedList<SharedStructDefinitions::StructElement*>* candidates;
		//Holds the padding number to make sure that theres no name collision.
		unsigned int paddingNumber;
		//Templated swap buffer. This can be done better but object pooling should help.
		DoublyLinkedList::Node<SharedStructDefinitions::StructElement*>* elementSwapBuffer;
		//Whether or not to define padding.
		bool padding;
	};
}

#endif