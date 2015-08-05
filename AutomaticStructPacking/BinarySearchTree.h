#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <iostream> //cerr.
#include <type_traits> //Used to help print errors.

namespace BinarySearchTree
{
	template <class T>
	struct Node
	{
		//Data of a node.
		T data;
		//Pointers to other elements of the tree.
		Node<T>* leftChild;
		Node<T>* rightChild;
	};

	template <class T>
	class BinarySearchTree
	{
	public:
		//Default Constructor.
		BinarySearchTree()
		{
			root = nullptr;
			count = 0;
		};
		//Default Destructor.
		~BinarySearchTree()
		{
			//TODO: Delete all objects.
			erase(root);
			count = 0;
			root = nullptr;
		};
		//Get Root.
		//@return Node<T>* - pointer to the start of the tree.
		inline Node<T>* getRoot() { return root; };
		//insert - overloaded for simplicity.
		//@param data - the data to put in the bst.
		void insert(T data)
		{
			insert(root, data);
		};
		//Removes an object from the tree.
		void remove(T data)
		{
			remove(root, data);
		}
		//Erases the tree.
		void erase(Node<T>* start)
		{
			if (start != nullptr)
			{
				erase(start->leftChild);
				erase(start->rightChild);
				delete start;
				count--;
			}
		}
		//insert - puts data into the tree.
		//@param root - the place to put the bst.
		void insert(Node<T>* root, T data)
		{
			//If a NULL root was fed.
			if (root == nullptr)
			{
				//Error safe.
				if (count != 0)
				{
					insert(this->root, data);
				}
				//Create the Root otherwise.
				else
				{
					Node<T>* n = new Node<T>();
					n->data = data;
					n->leftChild = nullptr;
					n->rightChild = nullptr;
					this->root = n;
					count++;
				}
			}
			else if (data < root->data)
			{
				if (root->leftChild != nullptr)
				{
					insert(root->leftChild, data);
				}
				else
				{
					root->leftChild = new Node < T > ;
					root->leftChild->data = data;
					root->leftChild->leftChild = nullptr;
					root->leftChild->rightChild = nullptr;
					count++;
				}
			}
			else if (data > root->data)
			{
				if (root->rightChild != nullptr)
				{
					insert(root->rightChild, data);
				}
				else
				{
					root->rightChild = new Node < T > ;
					root->rightChild->data = data;
					root->rightChild->leftChild = nullptr;
					root->rightChild->rightChild = nullptr;
					count++;
				}
			}
			else
			{
				//Something went horribly wrong.
				std::cerr << "An error occured inserting with data: " << data << std::endl;
			}
		};
		//Removes an object from the tree.
		void remove(Node<T>* root, T data)
		{
			Node<T>* current = root;
			Node<T>* parent = nullptr;
			while (current != nullptr)
			{
				if (current->data == data)
				{
					break;
				}
				else
				{
					parent = current;
					if (data > current->data)
						current = current->rightChild;
					if (data < current->data)
						current = current->leftChild;
				}
			}
			//Key not found.
			if (current == nullptr)
				return;
			//Case 1: Theres only one node.
			if (current->leftChild == nullptr && current->rightChild == nullptr)
			{
				if (parent->leftChild == current)
				{
					delete current;
					parent->leftChild = nullptr;
				}
				else if (parent->rightChild == current)
				{
					delete current;
					parent->rightChild = nullptr;
				}
				else
				{
					std::cerr << "Something went horribly wrong in deleting a one node child of data: " << data << std::endl;
					return;
				}
				count--;
			}
			//Case 2: Theres one child, to the left.
			else if (current->leftChild && current->rightChild == nullptr)
			{
				parent->leftChild = current->leftChild;
				delete current;
				count--;
			}
			//Case 3: Theres only one child, to the right.
			else if (current->rightChild && current->leftChild == nullptr)
			{
				parent->rightChild = current->rightChild;
				delete current;
				count--;
			}
			//Case 4: Theres two children.
			else if (current->rightChild && current->leftChild)
			{
				Node<T>* minimum = nullptr;
				if (current == root)
				{
					minimum = findMinimumNode(this->root, current);
				}
				else if (current == parent->leftChild || current == parent->rightChild)
				{
					minimum = findMinimumNode(current->leftChild, current);
				}
				else
				{
					std::cerr << "Something went wrong in case 4: " << data <<
						std::endl;
					return;
				}
				current->data = minimum->data;
				current->rightChild = minimum->rightChild;
				delete minimum;
				count--;
			}
			else
			{
				//Something went horribly wrong.
				std::cerr << "Unable to remove the node with data: " << data <<
					" : Most likely due to a pointer not being set properly." << std::endl;
				return;
			}
		}
		//Find the minimum node between these two points.
		//@param Node<T>* the minimum node.
		//@param Node<T>* the current node that will be deleted.
		Node<T>* findMinimumNode(Node<T>* minimum, Node<T>* current)
		{
			current = current->rightChild;
			if (current->leftChild == nullptr)
			{
				return current;
			}
			else
			{
				while (current->leftChild != nullptr)
				{
					current = current->leftChild;
				}
			}
			return current;
		}
		//Prints the tree, for debugging.
		//@param root - starts inorder from the root, pass NULL to start.
		void printInorder(Node<T>* root)
		{
			if (root != nullptr)
			{
				printInorder(root->leftChild);
				std::cout << root->data << std::endl;
				printInorder(root->rightChild);
			}
		}
		//Searches thte tree for a value.
		//@param data - the data to search
		Node<T>* search(T data)
		{
			Node<T>* current = root;
			if (current == nullptr)
			{
				std::cerr << "Value is not found." << std::endl;
				return nullptr;
			}
			return search(data, current);
		}
		//Searches the tree for a value.
		//@param data - the data to search for
		//@param Node<T>* - the location of where to search.
		Node<T>* search(T data, Node<T>* current)
		{
			if (current == nullptr)
			{
				return nullptr;
			}
			else if (current->data == data)
			{
				return current;
			}
			else if (current->data < data)
			{
				search(data, current->leftChild);
			}
			else if (current->data > data)
			{
				search(data, current->rightChild);
			}
			return nullptr;
		}
		//Exists - checks if the data exists.
		//@param data - the data to check for.
		bool exists(T data)
		{
			Node<T>* current = root;
			if (current == nullptr)
			{
				return false;
			}
			return exists(data, current);
		}
		//Exists - checks if the data exists.
		//@param data - the data to check for
		//@param current - the location where we are looking
		bool exists(T data, Node<T>* current)
		{
			if (current == nullptr)
			{
				return false;
			}
			else if (current->data == data)
			{
				return true;
			}
			else if (current->data < data)
			{
				search(data, current->leftChild);
			}
			else if (current->data > data)
			{
				search(data, current->rightChild);
			}
			return false;
		}
	private:
		//Private Variables.
		Node<T>* root;
		//Amount of elements in the tree.
		unsigned int count;
	};
}

#endif