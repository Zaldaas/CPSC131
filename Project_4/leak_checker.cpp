

//
#include <iostream>


//
#include "MyRolodex.hpp"
#include "MyHashTable.hpp"


//
using
	std::cout, std::endl
	;


void leaker()
{
	//
	CPSC131::MyRolodex::MyRolodex rolodex;
	CPSC131::MyHashTable::MyHashTable<int> table;
	
	//
	for ( int i = 0; i < 100; i++ ) {
		
		std::string key = "Test Key " + std::to_string(i);
		
		rolodex.add(
			key,
			"Address line 1" + std::to_string(i),
			"Address line 2" + std::to_string(i),
			"City " + std::to_string(i),
			"State " + std::to_string(i),
			i * 100,							// Zip
			"Country " + std::to_string(i)
		);
		table.add(key, i * 1293);
	}
	for ( int i = 0; i < 50; i++ ) {
		
		std::string key = "Test Key " + std::to_string(i);
		
		{
			auto val = rolodex.get(key);
		}
		rolodex.remove(key);
		
		table.get(key);
		table.remove(key);
	}
	
	//
	{
		CPSC131::MyRolodex::MyRolodex r1(rolodex);
		CPSC131::MyHashTable::MyHashTable<int> t1(table);
		
		auto r2 = rolodex;
		auto r3 = rolodex;
		auto t2 = table;
		auto t3 = table;
		
		r3.clear();
		t3.clear();
	}
}

//
int main()
{
	//
	cout << "Hello, my name is Niles Peppertrout!" << endl;
	
	//
	leaker();
	
	return 0;
}

