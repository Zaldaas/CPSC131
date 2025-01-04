#ifndef PUHP_TESTS_PROCESS_HPP
#define PUHP_TESTS_PROCESS_HPP


//
#include "./Files.hpp"
#include "./RAII_Pipe.hpp"


//
#include <cstring>
#include <fstream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/wait.h>
#include <tuple>
#include <unistd.h>
#include <vector>


//
namespace PuhPTests
{
	//
	struct ExecutionResult
	{
		int code = -1;
		std::string stdout = "";
		std::string stderr = "";
	};
	
	//
	class Process
	{
		//
		public:
			
			bool programExists(std::string name)
			{
				std::vector<std::string> args = {
					"which", name
				};
				auto result = this->execute(args);
				
				if ( result.code == 0 ) {
					return true;
				}
				
				return false;
			}
			
			/**
			 * Executes a command in a fork, while capturing the exit code, and stdout/stderr
			 * 
			 * The first argument MUST be the name of the program to execute
			 */
			ExecutionResult execute(std::vector<std::string> vArgs, std::string stdin = "")
			{
				//
				if ( vArgs.size() == 0 ) {
					throw std::runtime_error("Process::execute() - Cannot execute an empty argument list");
				}
				
				//	Grab the first argument, which is the program's name
				std::string program = vArgs[0];
				
				//
				ExecutionResult result;
				
				//	Setup pipes
				RAII_Pipe pipe_stdin, pipe_stdout, pipe_stderr;
				
				//	Fork a process to do the command
				pid_t child_pid = fork();
				
				//	Error
				if ( child_pid < 0 ) {
					throw std::runtime_error(std::string("Process::execute() - Unable to fork: ") + strerror(errno));
				}
				
				//	Parent
				else if ( child_pid > 0 ) {
					
					//	Close unneeded pipe ends
					pipe_stdin.closeRead();
					pipe_stdout.closeWrite();
					pipe_stderr.closeWrite();
					
					//	Send stdin to the forked process
					pipe_stdin.write(stdin);
					pipe_stdin.close();
					
					//	Wait for the child fork to finish,
					//	and close writing end of stdout/stderr pipes
					if ( waitpid(child_pid, &result.code, 0) < 0 ) {
						throw std::runtime_error(std::string("Process::execute() - waitpid has failed: ") + strerror(errno));
					}
					pipe_stdout.closeWrite();
					pipe_stderr.closeWrite();
					
					//	Read from the child's stdout and stderr pipes, then close them
					result.stdout = pipe_stdout.read();
					result.stderr = pipe_stderr.read();
					pipe_stdout.close();
					pipe_stderr.close();
				}
				
				//	Child
				else {
					
					//	Close unneeded pipe ends
					//	(we also don't want the executed program to see them)
					pipe_stdin.closeWrite();
					pipe_stdout.closeRead();
					pipe_stderr.closeRead();
					
					//	Redirect this child's stdin, stdout, and stderr
					//dup2(pipe_stdin.getReadEnd(), Process::FD_STDIN);
					//dup2(pipe_stdout.getWriteEnd(), Process::FD_STDOUT);
					//dup2(pipe_stderr.getWriteEnd(), Process::FD_STDERR);
					pipe_stdin.redirect_readFromStdin();
					pipe_stdout.redirect_writeToStdout();
					pipe_stderr.redirect_writeToStderr();
					
					//	Reformat arguments to match what execvp expects
					//	(don't worry about small memory leak; fork dies soon)
					//	Also don't forget the arguments need to be terminated
					//		by an additional nullptr
					char** cArgs = new char*[vArgs.size() + 1];
					for ( size_t i = 0; i < vArgs.size(); i++ ) {
						auto arg = vArgs[i];
						cArgs[i] = new char[arg.size() + 1];
						strcpy(cArgs[i], arg.c_str());
					}
					cArgs[ vArgs.size() ] = nullptr;
					
					//	Actually execute the process here in the child,
					//	with (hopefully) redirected output.
					if ( execvp(program.c_str(), cArgs) < 0 ) {
						throw std::runtime_error(std::string("Process::execute() - Failed to call execlp: ") + strerror(errno));
					}
					throw std::runtime_error("Process::execute() - Should to not have made it here (?)");
				}
				
				return result;
			}
			
		//
		private:
			
			//
			Files files_;
	};
}








#endif
