#ifndef PUHP_TESTS_TOSTRING_HPP
#define PUHP_TESTS_TOSTRING_HPP


//
#include <sstream>
#include <string>
#include <vector>


//
namespace PuhPTests
{
	/**
	 * Bring std::to_string into this namespace,
	 * so we can overload it
	 */
	using std::to_string;
	/**
	 * Overload to_string in this namespace so we can use it on vectors
	 * You can add your own to_string overloads in this namespace to help
	 * 	the tests properly render your items when needed.
	 */
	template <class T>
	std::string to_string(const std::vector<T>& v) {
		
		std::stringstream ss;
		bool first = true;
		
		ss << "{";
		for (auto item : v) {
			if (first) {
				first = false;
			}
			else{
				ss << ", ";
			}
			ss << "\"" << item << "\"";
		}
		ss << "}";
		
		return ss.str();
	}
	/// lol
	std::string to_string(const std::string& s) {
		return s;
	}
}

#endif

