#ifndef PUHP_TESTS_NET_HPP
#define PUHP_TESTS_NET_HPP


//
#include "./Files.hpp"
#include "./Process.hpp"
#include "./Random.hpp"
#include "./Utility.hpp"


//
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <tuple>


//
namespace PuhPTests
{
	//
	class Net
	{
		//
		public:
			
			//
			Net()
			{
					
			}
			
			//
			ExecutionResult sendData(std::string data, std::string hostname, int port, bool tcp = true)
			{
				//
				this->demandNetcat();
				
				//	Start arguments
				std::vector<std::string> args = {"nc"};
				if ( tcp == false ) {
					args.push_back("-u");
				}
				args.insert(args.end(),
				{
					"-w0",
					hostname,
					std::to_string(port)
				});
				
				//	Execute the command
				auto result = this->process_.execute(args, data);
				
				//	Check the result
				if ( result.code != 0 ) {
					throw std::runtime_error("Failed to send data with the following command: " + Utility::toString(args) + "\n==> " + strerror(errno));
				}
				
				return result;
			}
			
			//
			std::string receiveData(int port, int wait_seconds = 1, bool tcp = true)
			{
				//
				this->demandNetcat();
				
				//	Build the command
				std::vector<std::string> args = {
					"timeout",
					"--preserve-status", std::to_string(wait_seconds),
					"nc",
				};
				if ( tcp == false ) {
					args.push_back("-u");
				}
				args.insert(args.end(), {
					"-l",
					std::to_string(port)
				});
				
				//
				auto result = this->process_.execute(args);
				if ( false && result.code != 0 ) {	// I guess timeout always causes weird exit codes ?
					//std::cout << (
					throw std::runtime_error(
						std::string("receiveData() - Failed to receive data.")
						+ " Exit code = " + std::to_string(result.code)
						+ "\n> Command: " + Utility::toString(args)
						+ "\n> Error: " + strerror(errno)
						+ "\n> Stdout: " + result.stdout
						+ "\n> Stderr: " + result.stderr
						+ "\n"
					);
				}
				
				//
				//std::cout
				//	<< "Net::receiveData on port " << port << " got code " << result.code << ": " << std::endl
				//	<< "> " << result.stdout << std::endl
				//	<< "> " << result.stderr << std::endl
				//	;
				
				return result.stdout;
			}
			
		private:
			
			//
			Files files_;
			Process process_;
			
			//
			void demandProgram(std::string program)
			{
				//
				auto result = this->process_.execute({"which", program});
				if ( result.code != 0 ) {
					throw std::runtime_error("Progam '" + program + "' (nc) was not found on this system. Is it installed?");
				}
			}
			
			//
			void demandNetcat()
			{
				this->demandProgram("nc");
			}
	};
}




















#endif
