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
				//	Check that valgrind is actually installed
				{
					Process p;
					
					if ( p.programExists("valgrind") == false ) {
						throw std::runtime_error("Cannot check for memory leaks because valgrind is not installed !");
					}
				}
				
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
						"--leak-resolution=high",
						"--show-leak-kinds=definite",
						//"--track-origins=yes",	This option seems to make leak checks fail sometimes (?)
						"--error-exitcode=111",
						"--errors-for-leak-kinds=definite",
						"./" + exe_name
					}
				);
				
				stdout = result.stdout;
				stderr = result.stderr;
				
				if ( result.code != 0 ) {
					
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
