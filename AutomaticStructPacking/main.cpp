#include <iostream>
#include <string>
#include <Windows.h>

/*
	Overarching TODO's
	Teach the parser to recognize pointers and arrays.
	Make sure that the structs in the program are packed.
	Implement a threading class with the event handler (worker pool).
	Implement a GUI and have it do a async init.
*/

#include "Parser.h"

int main(int argc, char* argv[])
{
	Parser::Parser* myParser = new Parser::Parser();
	if (!myParser->init("test.txt"))
	{
		std::cout << "Parser was unable to open the file." << std::endl;
	}
	std::cout << "Parsing..." << std::endl;
	myParser->parseFile();
	myParser->parseTokens();
	myParser->dumpText(myParser->getFilename());
	Sleep(200000);
	//Test
	return 0;
}