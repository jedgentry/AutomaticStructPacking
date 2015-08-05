#ifndef PARSER_H
#define PARSER_H

#include <string> 
#include <fstream>
#include <functional>
#include <iostream> //cerr
#include <unordered_map> //hash table
#include <future> //Async

#include "BinarySearchTree.h"
#include "DoublyLinkedList.h"
#include "EventHandler.h"
#include "TokenMethods.h"

namespace Parser
{
	//Amount of memory to reserve for known filetypes.
	const unsigned int KNOWN_FILETYPES_RESERVE_AMOUNT = 4;
	//Where to load in supported filetypes.
	const std::string SUPPORTED_EXTENSION_FILENAME = "supported_filetypes.txt";
	//Whether or not to make a backup file.
	const bool MAKE_BACKUPS = true;

	class Parser
	{
	public:
		//Default Constructor.
		Parser();
		//Default Destructor
		~Parser();

		//Init the parser.
		//@param - the name of the file to initially open.
		//@return - true if initialization was successful.
		bool init(std::string filename);
		//Runs the parser.
		void parseFile();
		//Goes through the list for tokens.
		void parseTokens();
		//Checks if the token matches and does a function callback.
		//@param token - the string with the token.
		void checkIfToken(const std::string token, DoublyLinkedList::Node<std::string>* nodePtr);
		//Adds the tokens to the list.
		//@param token - the token to add to the list.
		void addToken(std::string token);
		//Adds the tokens to the list.
		//@param tokenList - the tokens to add to the list, seperated by a ' ' (whitespace).
		void addTokens(std::string tokenList);
		//Dumps the results back into a text file.
		//@param std::string - the filename to put the text out to.
		void dumpText(std::string destination);
		//Gets.
		//Returns the head of the list containig the raw text of the file.
		//@return DoublyLinkedList::DoublyLinkedList<std::string>* - pointer to the head of the raw text.
		DoublyLinkedList::DoublyLinkedList<std::string>* getRawText() { return rawText; };
		//Gets the filename that the parser has open.
		//@return std::string - the string containing the name of the file.
		std::string getFilename() { return filename; };
		//Sets.
		//Sets the input file of the parser.
		//@param filename - the name of the file to open.
		void setInputFile(std::string filename) { this->filename = filename; };
	private:
		//Loads supported file extensions.
		void loadSupportedFileExtensions();
		//Makes a backup of the file.
		void makeBackup();
		//Closes the file handle.
		void closeFile();
		//Opens the file.
		//@param std:;string - the filename to open.
		bool openFile(std::string filename);
		//Gets the results from the token methods.
		void getTokenResults();
		//Makes a backup filename of the current filename
		//@return std::string - the newly created backup filename
		std::string makeBackupFilename();
		//The file stream.
		std::ifstream inFileStream;
		//The filename we are currently loading.
		std::string filename;
		//The linked list containing the raw file in memory.
		DoublyLinkedList::DoublyLinkedList<std::string>* rawText;
		//Indexed keywords.
		BinarySearchTree::BinarySearchTree<std::string>* indexedKeywords;
		//The event handler that notifies with tokens.
		EventHandler::EventHandler<std::string>* eventHandler;
		//Vector containing the tokens.
		BinarySearchTree::BinarySearchTree<std::string>* tokens;
		//Token Method object for runtime callback.
		TokenMethods::TokenMethods* tokenMethods;
		//Current position using the node.
		DoublyLinkedList::Node<std::string>* current;
		//The hash table to verify that a token being called is a keyword.
	};
}

#endif