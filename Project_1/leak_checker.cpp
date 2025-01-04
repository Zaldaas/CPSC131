

//
#include <iostream>


//
#include "MyVector.hpp"


//
using
	std::cout, std::endl
	;


void leaker()
{
	//
	MyVector<int> v;
	
	//
	for ( int i = 0; i < 10000; i++ ) {
		v.push_back(i);
	}
	for ( int i = 0; i < 10000; i++ ) {
		v.pop_back();
	}
	
	//
	{
		MyVector<int> u = v;
	}
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

