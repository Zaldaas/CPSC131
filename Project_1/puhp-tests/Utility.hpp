#ifndef PUHP_TESTS_UTILITY_HPP
#define PUHP_TESTS_UTILITY_HPP


//
#include <sstream>
#include <vector>


//
namespace PuhPTests
{
	//
	class Utility
	{
		//
		public:
			
			//
			Utility()
			{
				//
			}
			
			//
			static std::string toString(const std::vector<std::string>& v)
			{
				//
				std::stringstream ss;
				
				//
				ss << "{";
				for ( size_t i = 0 ; i < v.size(); i++ ) {
					
					//
					if ( i > 0 ) {
						ss << ", ";
					}
					ss << v[i];
				}
				ss << "}";
				
				return ss.str();
			}
		
		//
		private:
			
			//
			
	};
}












#endif
