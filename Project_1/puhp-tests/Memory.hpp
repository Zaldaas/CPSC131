#ifndef PUHP_TESTS_MEMORY_HPP
#define PUHP_TESTS_MEMORY_HPP


//
#include "./Process.hpp"


//
#include <string>


//
namespace PuhPTests
{
	//
	class Memory
	{
		//
		public:
			
			//
			Memory()
			{
				//
			}
			
			//
			static bool checkMemoryLeaks(
				std::vector<std::string> compile_command, std::string exe_name,
				std::string& stdout, std::string& stderr, std::string& message
			)
			{
				//	Attempt to compile
				{
					Process p;
					ExecutionResult result = p.execute(compile_command);
					
					//	Only continue if compilation succeeds
					if ( result.code != 0 ) {
						
						stdout = result.stdout;
						stderr = result.stderr;
						message = "Compilation failed";
						
						return false;
					}
				}
				
				//	Execute the memory tester
				Process p;
				ExecutionResult result = p.execute(
					{
						"valgrind",
						"--tool=memcheck",
						"--leak-check=full",
						"--show-leak-kinds=definite",
						"--track-origins=yes",
						"--error-exitcode=111",
						"--errors-for-leak-kinds=definite",
						"./" + exe_name
					}
				);
				if ( result.code != 0 ) {
					
					stdout = result.stdout;
					stderr = result.stderr;
					message = "Memory leaks were detected";
					
					return false;
				}
				
				//
				message = "No leaks detected";
				
				return true;
			}
		
		//
		private:
			
			//
			
	};
}








#endif
