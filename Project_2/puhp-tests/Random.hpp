#ifndef PUHP_TESTS_RANDOM_HPP
#define PUHP_TESTS_RANDOM_HPP


//
#include <random>


//
namespace PuhPTests
{
	//	Seed the random generator engine
	//	Keep top of this namespace so it only has to be made once
	std::random_device random_device;
	
	//
	class Random
	{
		//
		public:
			
			//
			Random()
			{
				this->generator_ = std::mt19937(random_device());
			}
			
			//
			int get(int min, int max)
			{
				std::uniform_int_distribution<int> uniform_ints(min, max);
				int r = uniform_ints(this->generator_);
				
				return r;
			}
			
			//
			double get(double min = 0.0, double max = 1.0)
			{
				std::uniform_real_distribution<double> uniform_doubles(min, max);
				double r = uniform_doubles(this->generator_);
				
				return r;
			}
			
			//
			char getChar(bool includeUpper = false)
			{
				//
				char c = 'a';
				if ( includeUpper == true && this->getBool() == true ) {
					c = 'A';
				}
				
				//
				c += this->get(0, 25);
				
				return c;
			}
			
			//
			std::string getString(size_t length = 10, bool includeUpper = false)
			{
				//
				std::string s;
				
				//
				for ( size_t i = 0; i < length; i++ ) {
					s += this->getChar(includeUpper);
				}
				
				return s;
			}
			
			//
			bool getBool(double probability = 0.5)
			{
				//
				return this->get(0.0, 1.0) < probability;
			}
		
		//
		private:
			
			// Use the Mersenne-Twister engine for our random number generator
			std::mt19937 generator_;
	};
}

#endif
