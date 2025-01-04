

/**
 * TODO: Complete these class definitions
 */

/// Starter includes (YW)
#include "MyDetector.hpp"
#include "MyStack.hpp"
#include "MyQueue.hpp"

//
namespace CPSC131::PalindromeDetector
{
	/// YOUR WELCOME
	MyDetector::MyDetector() {}
	
	//
	bool MyDetector::isPalindrome(std::string s)
	{
		MyStack<char> stack;
        MyQueue<char> queue;

        for (char c : s)
        {
            if (isalpha(c))
            {
                char lower_c = tolower(c);
                stack.push(lower_c);
                queue.enqueue(lower_c);
            }
        }

        while (!stack.empty() && !queue.empty())
        {
            if (stack.top() != queue.front())
            {
                return false;
            }
            stack.pop();
            queue.dequeue();
        }

        return true;
	}
}












