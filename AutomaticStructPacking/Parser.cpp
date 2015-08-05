#include "Parser.h"

Parser::Parser::Parser()
{
	filename = "";
	rawText = new DoublyLinkedList::DoublyLinkedList < std::string > ;
	indexedKeywords = new BinarySearchTree::BinarySearchTree < std::string > ;
	eventHandler = new EventHandler::EventHandler < std::string > ;
	tokens = new BinarySearchTree::BinarySearchTree < std::string > ();
	tokenMethods = new TokenMethods::TokenMethods();
	current = nullptr;
	
}

Parser::Parser::~Parser()
{
	//Delete dynamically allocated variables.

}

bool Parser::Parser::init(std::string filename)
{
	//Load file types.
	loadSupportedFileExtensions();
	if (!openFile(filename))
	{
		std::cerr << "Unable to open the initial file: " << filename << std::endl;
		return false;
	}
	//Register event calls for functions.
	eventHandler->registerObserver("struct", std::bind(&TokenMethods::TokenMethods::tokenStruct, 
		tokenMethods, std::placeholders::_1));
	eventHandler->registerObserverResults("struct", std::bind(&TokenMethods::TokenMethods::tokenStructResult, 
		tokenMethods, std::placeholders::_1));
	//TODO: This is just temporary, eventually switch this to being a filetype.
	tokens->insert("struct");
	return true;
}

void Parser::Parser::addToken(std::string token)
{
	//Add token to callback list.
	tokens->insert(token);
}

void Parser::Parser::addTokens(std::string token)
{
	while (token.find(" "))
	{
		tokens->insert(token.substr(0, token.find(" ")));
		token = token.substr(token.find(" "), token.length());
	}
}

void Parser::Parser::parseFile()
{
	//Make a backup while we're reading the file.
	auto backupHandle = std::async(&Parser::makeBackup, this);
	std::string tempBuffer;
	while (inFileStream >> tempBuffer)
	{
		rawText->insertAtTail(tempBuffer);
	}
	backupHandle.wait();
}

void Parser::Parser::parseTokens()
{
	current = rawText->getHead();
	for (unsigned int i = 0; i < rawText->getCount(); i++)
	{
		if (current != nullptr)
		{
			checkIfToken(current->data, current);
			current = current->next;
		}
	}
	getTokenResults();
}

void Parser::Parser::checkIfToken(const std::string token, DoublyLinkedList::Node<std::string>* nodePtr)
{
	if (tokens->exists(token))
	{
		eventHandler->notifyWithNode(token, nodePtr);
	}
}

void Parser::Parser::dumpText(std::string destination)
{
	std::ofstream output;
	output.open(destination);
	if (!output.is_open())
	{
		std::cerr << "Unable to open the file: " << destination << std::endl;
		return;
	}
	else
	{
		DoublyLinkedList::Node<std::string>* current = rawText->getHead();
		while (current != nullptr)
		{
			output << current->data;
			current = current->next;
		}
		output.close();
	}
}

///////////////////////
// Private Functions //
///////////////////////

void Parser::Parser::makeBackup()
{
	if (!MAKE_BACKUPS)
		return;
	//Open initial file.
	std::ifstream backup;
	backup.open(filename);
	//Open backup file.
	std::ofstream outputBackup;
	std::string backupFilename = makeBackupFilename(), buffer = "";
	outputBackup.open(backupFilename);
	while (backup >> buffer)
	{
		outputBackup << buffer << std::endl;
	}
	//Close the output file.
	outputBackup.close();
	backup.close();
}

std::string Parser::Parser::makeBackupFilename()
{
	std::string backupFilename = filename;
	backupFilename = backupFilename.substr(0, backupFilename.find("."));
	backupFilename += "_bak";
	backupFilename += filename.substr(filename.rfind("."), filename.length());
	return backupFilename;
}

void Parser::Parser::getTokenResults()
{
	current = rawText->getHead();
	while (current != nullptr)
	{
		if (tokens->exists(current->data))
		{
			eventHandler->notifyResultsWithNode(current->data, current);
		}
	}
}

bool Parser::Parser::openFile(std::string filename)
{
	inFileStream.open(filename);
	if (!inFileStream.is_open())
	{
		std::cerr << "Unable to open the file: " << filename << std::endl;
		return false;
	}
	this->filename = filename;
	return true;
}

void Parser::Parser::closeFile()
{
	inFileStream.close();
	if (inFileStream.is_open())
	{
		std::cerr << "Unable to close the file." << std::endl;
	}
	filename = "";
}

void Parser::Parser::loadSupportedFileExtensions()
{
	std::ifstream supportedFileExtensionsStream;
	supportedFileExtensionsStream.open(SUPPORTED_EXTENSION_FILENAME);
	if (!supportedFileExtensionsStream.is_open())
	{
		std::cerr << "Unable to open the file: " << SUPPORTED_EXTENSION_FILENAME << std::endl;
		return;
	}
	std::string buffer = "";
	//TODO: Load this into a hash table.
}