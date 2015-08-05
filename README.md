# AutomaticStructPacking
Automates Struct Packing for C/C++ Languages by reorganizing elements to save space in memory while maintaining memory alignment.

Currently in development.

# Features
Struct Packing is an often forgot about technique used to reduce the memory footprint of a program. This is caused by compiler inserted padding in order to keep memory alignment for efficent reads when acccessing struct members. There are PERL scripts that can detect where padding was inserted by the compiler, however this is done after the fact and only suggests where to fix instead of tackling the problem. On the other hand, there are commands such as "\#pragma pack" however this causes memory to be unaligned, and causes performance degradation compared to aligned memory reads. This program tackles the problem from a parsing standpoint, and reads code, compares it against a list of known sizes, and then matches by biggest fit first.

# Design
The parser reads a file into a linked list and what will (soon be) a hash table that indexes the locations of keywords of interest. After the read, the parser will fire off events of the keywords from the (soon to be) hash table and use callbacks to process the tokens. Extra tokens can easily be added by sewing in your own Object, initializing it, adding it to the TokenMethods class and then binding it using std::bind in the Parser class.

# Methodology
Struct fields are quicksorted based on their size and then organized by biggest fit first, if no pair can be made, padding is explicitly defined for the user in order to easily suggest where free memory or a possible optimization can be made.

# Upcoming Features
I'm going to work on iterating through entire directories, adding multithreading, and asynchronous initialization when a GUI is implemented in the coming weeks. I'm also considering adding variadic functions to the event class once I finish experimenting with a few implementations.
