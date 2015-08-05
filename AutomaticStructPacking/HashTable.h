#ifndef HASHTABLE_H
#define HASHTABLE_H

#define HASHTABLE_INITIAL_SIZE 1024

#include "DoublyLinkedList.h"

namespace HashTable
{
	template <class T>
	class HashTable
	{
	public:
		//Default constructor.
		HashTable()
		{
			size = HASHTABLE_INITIAL_SIZE;
			m_hashTable = new DoublyLinkedList::DoublyLinkedList<T>[HASHTABLE_INITIAL_SIZE];
			key = HASHTABLE_INITIAL_SIZE;
			initHashTable();
		};
		//Overloaded constructor that sets the size of the hashtable.
		//@param size - the size to make the hash table.
		HashTable(unsigned int size)
		{
			this->size = size;
			m_hashTable = new DoublyLinkedList::DoublyLinkedList<T>[size];
			key = size;
			initHashTable();
		};
		//Overloaded constructor that sets the size and key of the table.
		//@param size - the size to make the hash table.
		//@param key - the key to make the table.
		HashTable(unsigned int size, unsigned int key)
		{
			this->size = size;
			m_hashTable = new DoublyLinkedList::DoublyLinkedList<T>[size];
			this->key = key;
			initHashTable();
		};
		//Default destructor.
		~HashTable()
		{
			for (unsigned int i = 0; i < size; i++)
			{
				//Clear dynamically allocated memory.
			}
		};
		//Insert into the hashTable
		//@param data - the data to insert.
		void insert(T data)
		{
			
		};
		//Remove - removes an object from the table.
		//@param data - the data to remove.
		void remove(T data)
		{

		};
		//Get's
		inline unsigned int getKey() { return key; };
		inline unsigned int getSize() { return size; };
		inline DoublyLinkedList::DoublyLinkedList<T>* getHashTable() { return m_hashTable; };
		//Set's
		inline void setKey(unsigned int key) { this->key = key; };
		inline void setSize(unsigned int size) { this->size = size; };
	private:
		//initHashTable - initilaizes the linked list in the hash table.
		void initHashTable()
		{
			for (unsigned int i = 0; i < size; i++)
			{
				m_hashTable[i] = new DoublyLinkedList::DoublyLinkedList<T>();
			}
		}
		//Private variables.
		unsigned int key;
		unsigned int size;
		DoublyLinkedList::DoublyLinkedList<T>* m_hashTable;
	};
}
#endif