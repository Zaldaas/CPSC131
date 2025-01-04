#ifndef PUHP_TESTS_TIMER_HPP
#define PUHP_TESTS_TIMER_HPP


//
#include <chrono>
#include <string>


//
namespace PuhPTests
{
	//
	class Timer
	{
		//
		public:
			
			//
			Timer()
			{
				//
				this->start();
			}
			Timer(const Timer& other)
			{
				this->copyOther(other);
			}
			
			//
			void start()
			{
				this->start_ = this->now();
				this->is_running_ = true;
			}
			
			//
			void stop()
			{
				this->end_ = this->now();
				this->is_running_ = false;
				
				this->calculateElapsed();
			}
			
			//
			std::chrono::time_point<std::chrono::high_resolution_clock> now()
			{
				return std::chrono::high_resolution_clock::now();
			}
			
			//
			std::chrono::duration<double> elapsed()
			{
				if ( this->is_running_ == true ) {
					return std::chrono::high_resolution_clock::now() - this->start_;
				}
				
				return this->end_ - this->start_;
			}
			
			long long microseconds() const
			{
				return this->elapsed_microseconds_;
			}
			
			std::string microseconds_s() const
			{
				return std::to_string(this->microseconds());
			}
			
			double milliseconds() const
			{
				return this->elapsed_milliseconds_;
			}
			
			std::string milliseconds_s() const
			{
				return std::to_string(this->milliseconds());
			}
			
			double seconds() const
			{
				return this->elapsed_seconds_;
			}
			
			std::string seconds_s() const
			{
				return std::to_string(this->seconds());
			}
			
			//
			Timer& operator=(const Timer& other)
			{
				return this->copyOther(other);
			}
		
		//
		private:
			
			//
			std::chrono::time_point<std::chrono::high_resolution_clock>
				start_,
				end_
				;
			
			//
			bool is_running_ = false;
			long long elapsed_microseconds_ = 0;
			double elapsed_milliseconds_ = 0.0;
			double elapsed_seconds_ = 0.0;
			
			//
			void calculateElapsed()
			{
				//
				auto elapsed = this->elapsed();
				this->elapsed_microseconds_ = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
				
				this->elapsed_milliseconds_ = static_cast<double>(this->elapsed_microseconds_) / 1000.0;
				this->elapsed_seconds_ = static_cast<double>(this->elapsed_microseconds_) / 1000000.0;
			}
			
			//
			Timer& copyOther(const Timer& other)
			{
				//
				this->start_ = other.start_;
				this->end_ = other.end_;
				
				//
				this->is_running_ = other.is_running_;
				this->elapsed_microseconds_ = other.elapsed_microseconds_;
				this->elapsed_milliseconds_ = other.elapsed_milliseconds_;
				this->elapsed_seconds_ = other.elapsed_seconds_;
				
				return *this;
			}
	};
}









#endif
