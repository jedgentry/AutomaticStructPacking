#ifndef LINKEDLIST
#define LINKEDLIST

#include <iostream> //for cerr

namespace LinkedList
{
	template <class T>
	struct Node
	{
		T data;
		Node<T>* next;
	};

	template <class T>
	class LinkedList
	{
	public:
		//Default constructor.
		LinkedList()
		{
			count = 0;
			head = nullptr;
			tail = nullptr;
		};
		//Default Destructor.
		~LinkedList()
		{
			clearList();
		};
		//Insert an item into the list.
		//@param data - the data to put in the list.
		void insertAtTail(T data)
		{
			Node<T>* n = new Node < T > ;
			n->data = data;
			n->next = nullptr;
			if (count == 0)
			{
				head = n;
				tail = n;
			}
			else if (count == 1)
			{
				head->next = n;
				tail = n;
			}
			else
			{
				tail->next = n;
				tail = n;
			}
			count++;
		};
		//Searches the list.
		//@param data - the data to search for
		//@return Node<T> - the result of the search
		Node<T>* search(T data)
		{
			Node<T>* current = head;
			if (current == nullptr)
			{
				return nullptr;
			}
			else
			{
				while (current != nullptr)
				{
					if (current->data == data)
					{
						return current;
					}
					current = current->next;
				}
			}
		};
		//Removes an item from the list
		//@param data - the data to remove from the list.
		void remove(T data)
		{
			if (current == nullptr)
			{
				return;
			}
			Node<T>* current = head, tail = nullptr;
			else
			{
				while (current != nullptr)
				{
					//We've found the node.
					if (current == data)
					{
						if (current->next != nullptr)
						{
							tail->next = current->next;
							delete current;
							count--;
							return;
						}
						else if (current->next == nullptr)
						{
							if (current == this->tail)
							{
								this->tail = tail;
								this->tail->next = nullptr;
								count--;
								delete current;
								return;
							}
							else
							{
								//Something went horribly wrong.
								std::cerr << "Remove had an error with next pointer in tail check."
									<< std::endl;
							}
						}
					}
					else
					{
						tail = current;
						current = current->next;
					}
				}
			}
		};
		//Clears the list and deallocates memory.
		void clearList()
		{
			Node<T>* current = head, tail = nullptr;
			if (current != nullptr)
			{
				while (current != nullptr)
				{
					tail = current;
					current = current->next;
					delete tail;
					count--;
				}
				head = nullptr;
				//could check if the current hit the tail to see
				//if it deleted everything.
				tail = nullptr;
			}
		};
		//Get's
		inline unsigned int getCount() { return count; };
		inline Node<T>* getHead() { return head; };
		inline Node<T>* getTail() { return tail; };
	private:
		//How many items are in the list
		unsigned int count;
		//The start of the list.
		Node<T>* head;
		Node<T>* tail;
	};
}

#endif