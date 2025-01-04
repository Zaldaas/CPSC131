

//
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


//
#include "MyVector.hpp"
#include "puhp-tests/PuhPTests.hpp"


//
using std::cout, std::cin, std::endl;
using std::string, std::to_string;
using std::vector;


//
using PuhPTests::Tests;
using PuhPTests::Test;


//
void preventPrototypeTampering()
{
	//	MyVector
	{
		//
		MyVector<int> t;
		
		//	Getters
		{
			std::function<size_t()> a;
			a = std::bind(&MyVector<int>::size, &t);
			a = std::bind(&MyVector<int>::capacity, &t);
			
			std::function<bool(std::string)> b;
			b = std::bind(&MyVector<int>::empty, &t);
			
			std::function<int&(size_t)> c;
			c = std::bind(&MyVector<int>::at, &t, std::placeholders::_1);
		}
		
		//	Setters
		{
			std::function<void(size_t)> a;
			a = std::bind(&MyVector<int>::reserve, &t, std::placeholders::_1);
			
			std::function<int&(size_t, const int&)> b;
			b = std::bind(&MyVector<int>::set, &t, std::placeholders::_1, std::placeholders::_2);
			
			std::function<int&(const int&)> c;
			c = std::bind(&MyVector<int>::push_back, &t, std::placeholders::_1);
			
			std::function<size_t()> d;
			d = std::bind(&MyVector<int>::pop_back, &t);
			
			std::function<int&(size_t, const int&)> e;
			e = std::bind(&MyVector<int>::insert, &t, std::placeholders::_1, std::placeholders::_2);
			
			std::function<size_t(size_t)> f;
			f = std::bind(&MyVector<int>::erase, &t, std::placeholders::_1);
			
			std::function<void()> g;
			g = std::bind(&MyVector<int>::clear, &t);
		}
	}
}


//
MyVector<int> makeVector(size_t count = 100)
{
	MyVector<int> v;
	for ( size_t i = 0; i < count; i++ ) {
		int val = i * 2 - (count / 2);
		v.push_back(val);
	}
	
	return v;
}

//
bool checkIntegerVectorsEqual(const MyVector<int>& v1, const MyVector<int>& v2)
{
	//
	if ( v1.size() != v2.size() ) {
		return false;
	}
	
	for ( size_t i = 0; i < v1.size(); i++ ) {
		if ( v1.at(i) != v2.at(i) ) {
			return false;
		}
	}
	
	return true;
}

//
bool checkIntegerVectorsEqual(const MyVector<int>& v1, const std::vector<int>& v2)
{
	//
	if ( v1.size() != v2.size() ) {
		return false;
	}
	
	for ( size_t i = 0; i < v1.size(); i++ ) {
		if ( v1.at(i) != v2.at(i) ) {
			return false;
		}
	}
	
	return true;
}

//
void printVector(const MyVector<int>& v, string label = "NoLabel") {
	if ( !label.empty() ) {
		cout << label << endl;
	}
	for (size_t i = 0; i < v.size(); i++) {
		cout << v.at(i) << endl;
	}
	cout << endl;
}

//
void testFreePoint(Tests& tests)
{
	//
	auto t = tests.createTest("Free point test(s)");
	
	//
	t->setFixedPointsPossible(1);
	
	//
	t->assertTrue(true, 1, "Free point for trying.");
}

//
void testPushBack(Tests& tests)
{
	//
	auto t = tests.createTest("PushBack tests");
	
	//
	t->setFixedPointsPossible(9);
	t->setNormalizedPointsPossibleTarget(5);
	
	//
	t->assertNoException(
		[t]()
		{
			auto v = makeVector(500);
			t->assertEqual(v.size(), 500ULL, 1, "Size should be 500 after push_back 500 elements.");
			t->assertEqual(v.capacity(), 512ULL, 1, "Check capacity");
			for ( int i : {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}) {
				v.push_back(i);
			}
			t->assertEqual(v.capacity(), 512ULL, 1, "Check capacity");
			v.push_back(5);
			t->assertEqual(v.capacity(), 1024ULL, 1, "Check capacity");
			t->assertEqual(v.size(), 513ULL, 1, "Size should be 513 after additional pushes");
			
		}, 1, "First half of push_back tests"
	);
	
	//
	t->assertNoException(
		[t]()
		{
			auto v = makeVector(1100);
			t->assertEqual(v.size(), 1100ULL, 1, "Size should be 1100 after push_back 1100 elements.");
			t->assertEqual(v.capacity(), 2048ULL, 1, "Capacity should be 2048 after pushing back 110 elements.");
			
		}, 1, "Second half of push_back tests"
	);
}

//
void testGrowShrink(Tests& tests)
{
	//
	auto t = tests.createTest("Grow/Shrink tests");
	
	//
	t->setFixedPointsPossible(14);
	t->setNormalizedPointsPossibleTarget(10);
	
	//
	t->assertNoException(
		[t]()
		{
			auto v = makeVector(0);
			t->assertEqual(v.size(), 0ULL, 1, "Size should be 0 initially.");
			t->assertEqual(v.capacity(), v.DEFAULT_CAPACITY, 1, "Check initial capacity.");
			
			//	Grow
			for ( int i = 0; i < 1023; i++ ) {
				v.push_back(i * 3);
			}
			t->assertEqual(v.capacity(), 1024ULL, 1, "Check edge-case capacity.");
			v.push_back(44444);
			t->assertEqual(v.capacity(), 1024ULL, 1, "Check edge-case capacity.");
			v.push_back(55555);
			t->assertEqual(v.capacity(), 2048ULL, 1, "Check edge-case capacity.");
			v.push_back(66666);
			t->assertEqual(v.capacity(), 2048ULL, 1, "Check edge-case capacity.");
			
			//	Shrink
			for ( int i = 0; i < 343; i++ ) {
				v.pop_back();
			}
			t->assertEqual(v.capacity(), 2048ULL, 1, "Check edge-case capacity at size " + to_string(v.size()) + " (shrink).");
			v.pop_back();
			t->assertEqual(v.capacity(), 2048ULL, 1, "Check edge-case capacity at size " + to_string(v.size()) + " (shrink).");
			v.pop_back();
			t->assertEqual(v.capacity(), 1024ULL, 1, "Check edge-case capacity at size " + to_string(v.size()) + " (shrink).");
			
			//	Grow
			for ( int i = 0; i < 20000; i++ ) {
				v.push_back(i * 3);
			}
			t->assertEqual(v.capacity(), 32768ULL, 1, "Check edge-case capacity.");
			
			//	Shrink
			for ( int i = 0; i < 9759; i++ ) {
				v.pop_back();
			}
			t->assertEqual(v.capacity(), 32768ULL, 1, "Check edge-case capacity (shrink).");
			v.pop_back();
			t->assertEqual(v.capacity(), 16384ULL, 1, "Check edge-case capacity (shrink).");
			v.pop_back();
			t->assertEqual(v.capacity(), 16384ULL, 1, "Check edge-case capacity (shrink).");
			
		}, 1, "Shrink tests"
	);
}

//
void testCapacity(Tests& tests)
{
	//
	auto t = tests.createTest("Capacity tests");
	
	//
	t->setFixedPointsPossible(5);
	t->setNormalizedPointsPossibleTarget(5);
	
	//
	t->assertNoException(
		[t]()
		{
			//	Make a MyVector with a mirror to a normal std::vector
			auto v = makeVector(1000);
			std::vector<int> sv;
			for ( size_t i = 0; i < v.size(); i++ ) {
				sv.push_back(v.at(i));
			}
			
			//
			v.reserve(2000);
			t->assertEqual(v.capacity(), 2000ULL, 1, "Capacity should match new 2000 value");
			t->assertTrue(checkIntegerVectorsEqual(v, sv), 2, "reserve() shouldn't have corrupted vector data");
			
			//
			v.reserve(1000);
			t->assertEqual(v.capacity(), 2000ULL, 1, "reserve() requests should not result in less capacity than already held");
			
		}, 1, "Capacity tests"
	);
}

//
void testRanges(Tests& tests)
{
	//
	auto t = tests.createTest("Range tests");
	
	//
	t->setFixedPointsPossible(22);
	t->setNormalizedPointsPossibleTarget(5);
	
	//	Try to grab invalid index
	t->assertException(
		[]()
		{
			auto v = makeVector(100);
			v.at(-1);
		}, 1, "Grab invalid index"
	);
	t->assertException(
		[]()
		{
			auto v = makeVector(100);
			v.at(100);
		}, 1, "Grab invalid index"
	);
	//	Try to grab valid index!
	t->assertNoException(
		[t]()
		{
			auto v = makeVector(100);
			t->assertEqual(v.at(0), -50, 1, "Check for correct number at index 0");
			t->assertEqual(v.at(35), 20, 1, "Check for correct number at index 35");
			t->assertEqual(v.at(99), 148, 1, "Check for correct number at index 99");
			
		}, 1, "Grab some values"
	);
	
	//	Try to insert at invalid index
	t->assertException(
		[]()
		{
			auto v = makeVector(100);
			v.insert(-1, 1);
		}, 1, "Insert at invalid index"
	);
	t->assertException(
		[]()
		{
			auto v = makeVector(100);
			v.insert(101, 1);
		}, 1, "Insert at invalid index 101"
	);
	
	//	Try to insert at valid index and check the result
	t->assertNoException(
		[t]()
		{
			auto v = makeVector(100);
			v.insert(77, 22);
			t->assertEqual(v.at(76), 102, 1, "Check for correct number at index 76");
			t->assertEqual(v.at(77), 22, 1, "Check for correct number at index 77");
			t->assertEqual(v.at(78), 104, 1, "Check for correct number at index 78");
		}, 1, "Grab invalid index"
	);
	
	//	Check that the [] operator works
	t->assertNoException(
		[t]()
		{
			//
			auto v = makeVector();
			t->assertEqual(v[1], -48, 1, "Check access operator [] at index 1");
			t->assertEqual(v[27], 4, 1, "Check access operator [] at index 27");
			t->assertEqual(v[97], 144, 1, "Check access operator [] at index 97");
			
		}, 1, "Check access operator"
	);
	
	//	Check that the [] operator throws for invalid index -1
	t->assertException(
		[]()
		{
			auto v = makeVector();
			v[-1];
			
		}, 1, "Check access operator throws for index -1"
	);
	//	Check that the [] operator throws for invalid index 100
	t->assertException(
		[]()
		{
			auto v = makeVector();
			v[100];
			
		}, 1, "Check access operator throws for index 100"
	);
	
	// Check pop_back
	t->assertNoException(
		[t]()
		{
			//
			auto v = makeVector(1);
			v.pop_back();
			t->assertTrue(v.empty(), 1, "Single-element vector should become empty after pop_back");
			
			//
			v.push_back(1);
			v.push_back(2);
			v.pop_back();
			t->assertTrue(v.size() == 1ULL && v[0] == 1, 1, "Checking pop_back logic");
			
		}, 1, "Valid pop_back operations shouldn't throw an exception"
	);
	t->assertException(
		[]()
		{
			auto v = makeVector(0);
			v.pop_back();
			
		}, 1, "Pop back should throw on an empty vector"
	);
}

//
void testCopying(Tests& tests)
{
	//
	auto t = tests.createTest("Copy tests");
	
	//
	t->setFixedPointsPossible(7);
	t->setNormalizedPointsPossibleTarget(5);
	
	//
	t->assertNoException(
		[t]()
		{
			//
			auto v1 = makeVector();
			MyVector<int> v2, v3(v1);
			
			//
			v2 = v1;
			
			//
			t->assertTrue(checkIntegerVectorsEqual(v1, v2), 1, "Assignment operator (equality check)");
			t->assertTrue(v1.size() == v2.size(), 1, "Assignment operator (size)");
			t->assertTrue(v1.capacity() == v2.capacity(), 1, "Assignment operator (size)");
			
			//
			t->assertTrue(checkIntegerVectorsEqual(v1, v3), 1, "Copy constructor (equality check)");
			t->assertTrue(v1.size() == v3.size(), 1, "Copy constructor (size)");
			t->assertTrue(v1.capacity() == v3.capacity(), 1, "Copy constructor (size)");
			
		}, 1, "Copy tests"
	);
}

//
void testDeletions(Tests& tests)
{
	//
	auto t = tests.createTest("Deletion tests");
	
	//
	t->setFixedPointsPossible(8);
	t->setNormalizedPointsPossibleTarget(5);
	
	//
	t->assertNoException(
		[t]()
		{
			//
			auto v = makeVector(100);
			
			//
			v.erase(47);
			t->assertTrue(v.size() == 99ULL, 1, "Erase an element in the middle (size)");
			t->assertTrue(v.capacity() == 128ULL, 1, "Erase an element in the middle (capacity)");
			t->assertEqual(v.at(88), 128, 1, "Erase an element in the middle (check another element)");
			
			//
			v.erase(0);
			t->assertTrue(v.size() == 98ULL, 1, "Erase first element (size)");
			t->assertTrue(v.capacity() == 128ULL, 1, "Erase first element (capacity)");
			t->assertEqual(v.at(88), 130, 1, "Erase first element (check another element)");
			
		}, 1, "First half of deletion tests"
	);
	
	//
	t->assertException(
		[]()
		{
			//
			auto v = makeVector(1);
			
			//
			v.erase(0);
			v.erase(0);
			
		}, 1, "Erasing 2 elements from a 1 element vector should throw exception"
	);
}

//
void testDestructor(Tests& tests)
{
	//
	auto t = tests.createTest("Destructor tests");
	
	//
	t->setFixedPointsPossible(3);
	t->setNormalizedPointsPossibleTarget(5);
	
	//	Local classes, lel
	class MyThrowner
	{
		// hai
	};
	class MyThrower
	{
		public:
			~MyThrower() noexcept(false) {
				if (this->throw_enabled == true) {
					throw MyThrowner();
				}
			}
			bool throw_enabled = false;
	};
	
	//
	t->assertException(
		[]()
		{
			//
			MyVector<MyThrower> v;
			v.push_back(MyThrower());
			v[0].throw_enabled = true;
			
			//
			try
			{
				v.clear();
			}
			catch(const MyThrowner& e)
			{
				// yey
				v[0].throw_enabled = false;
				throw;
			}
			catch( ... )
			{
				// oh nyo
			}
			v[0].throw_enabled = false;
			
		}, 1, "Vector should call element destructors when clear()ing"
	);
	
	//
	t->assertException(
		[]()
		{
			//
			MyVector<MyThrower> v;
			v.push_back(MyThrower());
			v.push_back(MyThrower());
			v[0].throw_enabled = true;
			v[1].throw_enabled = true;
			
			//
			try
			{
				v.erase(0);
			}
			catch(const MyThrowner& e)
			{
				// yey
				v[0].throw_enabled = false;
				v[1].throw_enabled = false;
				throw;
			}
			catch( ... )
			{
				// oh nyo
			}
			v[0].throw_enabled = false;
			v[1].throw_enabled = false;
			
		}, 1, "Vector should call element destructors when erasing()ing"
	);
	
	//
	t->assertException(
		[]()
		{
			//
			MyVector<MyThrower> v;
			v.push_back(MyThrower());
			v.push_back(MyThrower());
			v[0].throw_enabled = true;
			v[1].throw_enabled = true;
			
			//
			try
			{
				v.set(1, MyThrower());
			}
			catch(const MyThrowner& e)
			{
				// yey
				v[0].throw_enabled = false;
				v[1].throw_enabled = false;
				throw;
			}
			catch( ... )
			{
				// oh nyo
			}
			v[0].throw_enabled = false;
			v[1].throw_enabled = false;
			
		}, 1, "Vector should call element destructors when set()ing"
	);
}

//
void testEmpty(Tests& tests)
{
	//
	auto t = tests.createTest("Empty tests");
	
	//
	t->setFixedPointsPossible(5);
	t->setNormalizedPointsPossibleTarget(5);
	
	//
	t->assertNoException(
		[t]()
		{
			//
			auto v = makeVector(0);
			
			//
			t->assertTrue(v.empty(), 1, "empty() should work with empty vector");
			
			//
			v.push_back(1);
			t->assertFalse(v.empty(), 1, "empty() should not work with non-empty vector");
			
			//
			v.erase(0);
			t->assertTrue(v.empty(), 1, "empty() should work with empty vector (again)");
			
			//
			v.push_back(1);
			v.push_back(1);
			v.push_back(1);
			v.push_back(1);
			v.push_back(1);
			v.clear();
			t->assertTrue(v.empty(), 1, "empty() should work with empty vector (again)");
			
		}, 1, "Empty tests"
	);
}

//
void testPointerStealing(Tests& tests)
{
	//
	auto t = tests.createTest("Pointer Stealing");
	
	//
	t->setFixedPointsPossible(11);
	t->setNormalizedPointsPossibleTarget(5);
	
	//
	t->assertNoException(
		[t]()
		{
			//
			auto v = makeVector(1);
			for ( int i = 0; i < 1000; i++ ) {
				v.push_back(i * 5);
			}
			auto u = v;
			
			//
			u[333]--;
			
			//
			t->assertFalse(v[333] == u[333], 10, "Checking for copying that engages in pointer-stealing");
			
		}, 1, "Memory leaks"
	);
}

//
void testMemoryLeaks(Tests& tests)
{
	//
	auto t = tests.createTest("Memory Leaks");
	
	//
	t->setFixedPointsPossible(11);
	t->setNormalizedPointsPossibleTarget(5);
	
	//
	t->assertNoException(
		[t]()
		{
			//
			string stdout, stderr, message;
			bool leakCheckResult = PuhPTests::Memory::checkMemoryLeaks
			(
				{"make", "leak-checker"},
				"leak-checker",
				stdout, stderr, message
			);
			
			//
			if ( !leakCheckResult ) {
				t->log("Leak Check Result :: STDOUT\n" + stdout);
				t->log("Leak Check Result :: STDERR\n" + stdout);
			}
			t->assertTrue(leakCheckResult, 10, "Checking for memory leaks: " + message);
			
		}, 1, "Memory leaks"
	);
}

//
int main()
{
	//
	Tests tests(50);
	
	//
	preventPrototypeTampering();
	
	//
	std::vector<std::function<void(Tests&)>> fs = {
		testFreePoint,
		testPushBack,
		testGrowShrink,
		testRanges,
		testCapacity,
		testDeletions,
		testCopying,
		testDestructor,
		testEmpty,
		testPointerStealing,
		testMemoryLeaks
	};
	
	//
	for ( auto f : fs ) {
		f(tests);
	}
	
	//
	tests.run();
	
	//
	cout << "Tests complete" << endl;
	
	return 0;
}





