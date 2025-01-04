

//
#include <iostream>


//
#include "DoublyLinkedList.hpp"


//
using
	std::cout, std::endl
	;


void leaker()
{
	//
	CPSC131::DoublyLinkedList::DoublyLinkedList<int> dll;
	
	//
	for ( int i = 0; i < 100; i++ ) {
		dll.push_front(i * 2);
		dll.push_back(i * 2);
	}
	for ( int i = 0; i < 100; i++ ) {
		dll.pop_front();
		dll.pop_back();
	}
	
	dll.push_front(5);
	dll.erase(dll.begin());
	
	dll.assign(276, 100);
	
	//
	{
		// cout << "Enter assignment operator overload" << endl;
		CPSC131::DoublyLinkedList::DoublyLinkedList<int> t1 = dll;
		CPSC131::DoublyLinkedList::DoublyLinkedList<int> t2(dll);
	}
	
	//
	dll.assign(100, 1000);
}

//
int main()
{
	//
	cout << "Hello, my name is Quiche Hollandaise!" << endl;
	
	//
	leaker();
	
	return 0;
}

