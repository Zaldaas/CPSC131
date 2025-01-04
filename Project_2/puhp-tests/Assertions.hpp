#ifndef PUHP_TESTS_ASSERTIONS_HPP
#define PUHP_TESTS_ASSERTIONS_HPP


//
#include "./Enums.hpp"


//
#include <functional>
#include <stdexcept>
#include <string>


//
namespace PuhPTests
{
	//
	class Assertions
	{
		//
		public:
			Assertions(){}
			
			//
			static std::string typeToString(AssertionType t)
			{
				switch(t)
				{
					case AssertionType::True:
						return "True";
					case AssertionType::False:
						return "False";
					case AssertionType::Equal:
						return "Equal";
					case AssertionType::NotEqual:
						return "NotEqual";
					case AssertionType::Exception:
						return "Exception";
					case AssertionType::NoException:
						return "NoException";
				}
				
				throw std::runtime_error("Assertions::typeToString(): Unsupported assertion type");
			}
			
			/**
			 * Attempts to run the passed lambda "func".
			 * Returns "true" is an exception was encountered, false otherwise
			 */
			static bool assertException(const std::function<void()>& func, std::string& out_message)
			{
				//
				bool caught_exception = false;
				std::string exception_message = "";
				
				//
				out_message = "";
				
				//
				try
				{
					func();
					
				}
				catch(const std::exception& e)
				{
					caught_exception = true;
					exception_message = e.what();
				}
				catch( ... )
				{
					caught_exception = true;
					exception_message = "Unknown exception type";
				}
				
				//
				if ( caught_exception == true ) {
					out_message = "Caught exception :: " + exception_message;
				}
				else {
					out_message = "Did not catch exception";
				}
				
				return caught_exception;
			}
			
			/**
			 * Attempts to run the passed lambda "func".
			 * Returns "true" is NO exception was encountered, false otherwise
			 */
			static bool assertNoException(const std::function<void()>& func, std::string& out_message)
			{
				bool returnValue = !Assertions::assertException(func, out_message);
				
				return returnValue;
			}
			
			/**
			 * Return true if boolean is true
			 */
			static bool assertTrue(bool b, std::string& out_message)
			{
				out_message = b ? "TRUE" : "NOT TRUE";
				return b;
			}
			
			/**
			 * Return true if boolean is false
			 */
			static bool assertFalse(bool b, std::string& out_message)
			{
				out_message = b ? "NOT FALSE" : "FALSE";
				return !b;
			}
			
			/**
			 * Return true if the two items are "equal"
			 * 
			 * Equality is determined by the existing comparison operator between the two types,
			 * 	which you'll want to make sure are properly overloaded/defined.
			 * 
			 * Works with any two classes that have properly implemented comparison operators.
			 * 
			 * Will also attempt to print out the two items using to_string.
			 * Above, std::to_string is brought into the current namespace,
			 * 	which means you can overload simply "to_string" in this namespace to add support
			 * 	for additional types. See the overload for std::vector near the top of this source file.
			 */
			template<class T1, class T2>
			static bool assertEqual(T1 a, T2 b, std::string& out_message)
			{
				//
				bool is_equal = (a == b);
				std::string comparison_label = (is_equal ? " == " : " != ");
				out_message = "";
				
				//	Only output message if the two items are not equal
				if ( is_equal == false ) {
					out_message = "<<<" + to_string(a) + ">>>" + comparison_label + "<<<" + to_string(b) + ">>>";
				}
				
				return is_equal;
			}
			
			/**
			 * Opposite of assertEqual
			 */
			template<class T1, class T2>
			static bool assertNotEqual(T1 a, T2 b, std::string& out_message)
			{
				//
				bool is_equal = (a == b);
				std::string comparison_label = (is_equal ? " == " : " != ");
				out_message = "";
				
				//	Only output message if the two items are equal
				if ( is_equal == true ) {
					out_message = "<<<" + to_string(a) + ">>>" + comparison_label + "<<<" + to_string(b) + ">>>";
				}
				
				return !is_equal;
			}
	};
}















#endif
