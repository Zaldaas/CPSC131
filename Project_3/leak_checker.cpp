

//
#include <iostream>


//
#include "MyStack.hpp"
#include "MyQueue.hpp"
#include "MyDetector.hpp"


//
using
	std::cout, std::endl
	;


void leaker()
{
	//
	CPSC131::MyStack<int> stack;
	CPSC131::MyQueue<int> q;
	CPSC131::PalindromeDetector::MyDetector detector;
	
	//
	for ( int i = 0; i < 100; i++ ) {
		stack.push(i * 2);
		q.enqueue(i * 2);
	}
	for ( int i = 0; i < 50; i++ ) {
		stack.top();
		stack.pop();
		q.front();
		q.dequeue();
	}
	stack.clear();
	q.clear();
	
	//
	detector.isPalindrome("Uhm hai");
	detector.isPalindrome("radar");
	detector.isPalindrome("");
	
	//
	{
		CPSC131::MyStack<int> st1 = stack;
		CPSC131::MyStack<int> st2(stack);
		
		CPSC131::MyQueue<int> q1 = q;
		CPSC131::MyQueue<int> q2(q);
	}
}

//
int main()
{
	//
	cout << "Hello, my name is Gibsen Montgomery Gibsen!" << endl;
	
	//
	leaker();
	
	return 0;
}

