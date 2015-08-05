#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream> //print

namespace DoublyLinkedList
{
	template <class T>
	struct Node
	{
		T data;
		Node<T>* next;
		Node<T>* previous;
	};

	template <class T>
	class DoublyLinkedList
	{
	public:
		//Standard construtor.
		DoublyLinkedList()
		{
			count = 0;
			tail = nullptr;
			head = nullptr;
		};
		//Insert - puts a node into the end of the list.
		//@param data - the data to put in the list.
		void insertAtTail(const T data)
		{
			Node<T>* n = new Node < T >;
			n->data = data;
			if (count == 0)
			{
				n->next = nullptr;
				n->previous = nullptr;
				head = n;
				tail = n;
			}
			else if (count == 1)
			{
				n->next = nullptr;
				n->previous = head;
				head->next = n;
				tail = n;
			}
			else
			{
				n->next = nullptr;
				n->previous = tail;
				tail->next = n;
				tail = n;
			}
			count++;
		};
		//Insert - puts a node into the start of the list.
		//@param data - the data to put in the list.
		void insertAtHead(const T data)
		{
			Node<T>* n = new Node < T >;
			n->data = data;
			if (count == 0)
			{
				n->next = nullptr;
				n->previous = nullptr;
				head = n;
				tail = n;
			}
			else if (count == 1)
			{
				head = n;
				head->next = tail;
				head->previous = nullptr;
				tail->previous = head;
			}
			else
			{
				head->previous = n;
				n->next = head;
				n->previous = nullptr;
				head = n;
			}
			count++;
		};
		//Remove - removes a node from the list.
		//@param data - the data to remove.
		void remove(const T data)
		{
			if (head->data == data)
			{
				removeHead();
			}
			else if(tail->data == data)
			{
				removeTail();
			}
			else
			{
				Node<T>* n = search(data);
				n->previous->next = n->next;
				n->next->previous = n->previous;
				count--;
			}
		};
		//Remove - removes a node from the list.
		//@param Node* - the pointer to remove.
		void remove(const Node<T>* pointer)
		{
			remove(pointer->data);
		};
		//Deletes from the first node in the list.
		void removeHead()
		{
			if (count == 0 || head == nullptr)
			{
				return;
			}
			else if (count == 1)
			{
				remove(head);
			}
			else
			{
				Node<T>* prevHead = head;
				head = head->next;
				delete prevHead;
				head->previous = nullptr;
			}

		}
		//Deletes from the last node in the list.
		void removeTail()
		{
			if (count == 0 || tail == nullptr)
			{
				return;
			}
			else if (count == 1)
			{
				remove(tail);
			}
			else
			{
				Node<T>* prevTail = tail;
				tail = tail->previous;
				delete prevTail;
				tail->next = nullptr;
			}
		}
		//Search - searches the list by data.
		//@param data - the data to search for in the list
		//@return Node* - pointer to the node that was searched for.
		Node<T>* search(const T data)
		{
			Node<T>* n = head;
			while (n->next != nullptr && data != n->data)
			{
				n = n->next;
			}
			return n;
		};
		//Search - searches the list by data in reverse.
		//@param data - the data to search for in the list
		//@return Node* - pointer to the node that was searched for.
		Node<T>* reverseSearch(const T data)
		{
			Node<T>* n = tail;
			while (n->previous != nullptr && data != n->data)
			{
				n = n->previous;
			}
			return n;
		};
		//getCount - returns the number of nodes in the list.
		//@return unsigned int - number of nodes in the list.
		unsigned int getCount() { return count; };
		//Get Tail - returns the end of the list.
		//@return Node* - pointer to the end of the list.
		Node<T>* getTail() { return tail; };
		//Get Head - returns the start of the list.
		//@return Node* - the pointer to the start of the list.
		Node<T>* getHead() { return head; };
		//Debug prints the nodes.
		void print()
		{
			Node<T>* n = head;
			unsigned int i = 0;
			if (count > 1)
			{
				while (n->next != nullptr)
				{
					std::cout << i + 1 << ": " << n->data << std::endl;
					n = n->next;
					i++;
				}
				std::cout << i + 1 << ": " << n->data << std::endl;
			}
			else
			{
				std::cout << i << ": " << n->data << std::endl;
			}
			if (i != count - 1)
			{
				std::cerr << "Error: printed nodes do not match the total number of nodes." << std::endl;
			}
		};
		//Clears the List and resets the class.
		void clearList()
		{
			Node<T>* current = head;
			Node<T>* tail = nullptr;
			if (current == nullptr)
			{
				return;
			}
			else
			{
				while (current != nullptr)
				{
					tail = current;
					current = current->next;
					delete tail;
					count--;
				}
			}
		};
		//Default Destructor.
		~DoublyLinkedList()
		{
			clearList();
			count = 0;
			tail = nullptr;
			head = nullptr;
		};
	private:
		//Pointer to the start of the list.
		Node<T>* head;
		//Pointer to the end of the list.
		Node<T>* tail;
		//The amount of nodes in the list.
		unsigned int count;
	};
}
#endif