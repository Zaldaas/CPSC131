#ifndef PUHP_TESTS_COMPLEXITY_HPP
#define PUHP_TESTS_COMPLEXITY_HPP


//
#include "./Timer.hpp"


//
#include <cmath>
#include <iostream>
#include <sstream>


//
namespace PuhPTests
{
	//
	class Complexity
	{
		//
		public:
			
			static constexpr double DEFAULT_PREDICTION_TOLERANCE = 0.2;
			static constexpr bool DEFAULT_VERBOSE = false;
			
			//
			Complexity(double tolerance = Complexity::DEFAULT_PREDICTION_TOLERANCE, bool verbose = Complexity::DEFAULT_VERBOSE)
			{
				this->tolerance_ = tolerance;
				this->verbose_ = verbose;
			}
			
			//
			void verbose(bool b)
			{
				this->verbose_ = b;
			}
			bool verbose()
			{
				return this->verbose_;
			}
			
			//
			bool checkConstantTime(Timer t1, size_t n1, Timer t2, size_t n2)
			{
				bool result = this->checkPrediction(
					static_cast<double>(t1.microseconds()),
					static_cast<double>(t2.microseconds())
				);
				
				return result;
			}
			
			//
			bool checkLinear(Timer t1, size_t n1, Timer t2, size_t n2)
			{
				//
				std::cout << "WARNING: PuhPTests::Complexity::checkLinear() is untested!" << std::endl;
				
				//
				double microsecondsPerN1 = this->microsecondsPerN(t1, n1);
				double microsecondsPrediction = microsecondsPerN1 * n2;
				
				//
				return this->checkPrediction(
					static_cast<double>(t1.microseconds()),
					static_cast<double>(microsecondsPrediction)
				);
			}
			
			//
			bool checkPolynomial(Timer t1, size_t n1, Timer t2, size_t n2, double power)
			{
				std::cout << "WARNING: PuhPTests::Complexity::checkPolynomial() is untested!" << std::endl;
				
				/**
				 * Start with t = (n^power) + Y
				 * Solve for Y?
				 * 
				 * Y = t1 / (n1^power)
				*/
				double y =
					static_cast<double>(t1.microseconds())
					/
					pow(static_cast<double>(n1), power)
					;
				
				//
				double prediction = pow(static_cast<double>(n2), power) + y;
				
				//
				bool result = this->checkPrediction(
					static_cast<double>(t2.microseconds()),
					prediction
				);
				
				return result;
			}
		
		//
		private:
			
			//
			double tolerance_ = 0.0;
			bool verbose_ = false;
			
			//
			double microsecondsPerN(const Timer& t, size_t n)
			{
				//
				if ( n == 0 ) {
					return 0.0;
				}
				
				//
				double microseconds = static_cast<double>(t.microseconds());
				microseconds /= static_cast<double>(n);
				
				return microseconds;
			}
			
			//
			bool checkPrediction(double actual, double predicted)
			{
				//
				if ( actual == 0.0 ) {
					
					if ( this->verbose_ ) {
						std::cout << "PuhPTests::Complexity::checkPrediction() - Actual was 0.0; Returning false" << std::endl;
					}
					
					return false;
				}
				
				//
				double difference = abs(actual - predicted);
				double error = difference / actual;
				
				//
				bool result = (error <= this->tolerance_);
				
				//
				std::stringstream ss;
				ss
					<< "PuhPTests::Complexity::checkPrediction() - Prediction " << (result ? "correct" : "incorrect")
					<< " (" << predicted << " vs. " << actual << ")"
					<< "; Error " << (error * 100) << "% " << (result ? "within" : "outside") << " tolerance of " << (this->tolerance_ * 100) << "%"
					<< std::endl
					;
				if ( this->verbose_ || result == false ) {
					std::cout << "> " << ss.str() << std::endl;
				}
				
				return result;
			}
	};
}











#endif
