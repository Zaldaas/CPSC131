#ifndef PUHP_TESTS_OUTPUTCAPTURE_HPP
#define PUHP_TESTS_OUTPUTCAPTURE_HPP


//
#include <iostream>
#include <sstream>
#include <string>


//
namespace PuhPTests
{
	/**
	 * This class attempts to capture output from cin and cerr (stdout and stderr)
	 * As of 2022-03-09, it won't capture output from printf()
	 * Begins capturing on creation.
	 */
	class OutputCapture
	{
		//
		public:
			
			//
			OutputCapture(bool defer_capture = false)
			{
				if ( !defer_capture ) {
					this->beginCapture();
				}
			}
			
			~OutputCapture()
			{
				this->endCapture();
			}
			
			/// Fetch the stdout output that has been captured so far
			std::string getStdout()
			{
				this->flush();
				return this->stdout_buffer.str();
			}
			/// Fetch the stderr output that has been captured so far
			std::string getStderr()
			{
				this->flush();
				return this->stderr_buffer.str();
			}
			
			/// Start capturing stdout and stderr
			void beginCapture()
			{
				this->endCapture();
				
				this->stdout_buffer.str("");
				this->old_stdout = std::cout.rdbuf(this->stdout_buffer.rdbuf());
				std::cout.rdbuf(this->stdout_buffer.rdbuf());
				
				this->stderr_buffer.str("");
				this->old_stderr = std::cerr.rdbuf(this->stderr_buffer.rdbuf());
				std::cerr.rdbuf(this->stderr_buffer.rdbuf());
			}
			
			/// Stop capturing stdout and stderr (but keep a copy of what was captured so far).
			void endCapture()
			{
				//
				this->flush();
				
				if ( this->old_stdout != nullptr ) {
					std::cout.rdbuf(this->old_stdout);
					this->old_stdout = nullptr;
				}
				if ( this->old_stderr != nullptr ) {
					std::cerr.rdbuf(this->old_stderr);
					this->old_stderr = nullptr;
				}
			}
			
			/// Make sure cout and cerr have been flushed, so they're actually captured by this class.
			void flush()
			{
				std::cout << std::flush;
				std::cerr << std::flush;
			}
			
			///	Take a string and inject it into the cin buffer, so it becomes the next thing read.
			static void injectToCin(std::string s)
			{
				for ( size_t i = 0; i < s.size(); i++ ) {
					size_t pos = s.size() - i - 1;
					std::cin.putback(s[pos]);
				}
			}
			
		private:
			
			//
			std::stringstream stdout_buffer;
			std::stringstream stderr_buffer;
			
			//
			std::streambuf* old_stdout = nullptr;
			std::streambuf* old_stderr = nullptr;
	};
}

#endif
