#ifndef PUHP_TESTS_FILES_HPP
#define PUHP_TESTS_FILES_HPP


//
#include "./Random.hpp"


//
#include <fstream>
#include <string>


//
namespace PuhPTests
{
	//
	class Files
	{
		//
		public:
		
			//
			static const size_t BUFFER_SIZE = 8192;
			
			//
			Files()
			{
					
			}
			
			//
			std::string randomFileName(std::string prefix, std::string extension = "")
			{
				auto randomNumber = this->random_.get(1000000000, 2000000000);
				std::string file_name = std::string(".") + prefix + "." + std::to_string(randomNumber);
				if ( !extension.empty() ) {
					file_name += "." + extension;
				}
				
				return file_name;
			}
			
			//
			std::string readFile(std::string file_name, bool binary = false)
			{
				//
				auto flags = std::ifstream::in;
				if ( binary == true ) {
					flags |= std::ifstream::binary;
				}
				
				std::ifstream f(file_name, flags);
				if ( !f ) {
					throw std::runtime_error("Unable to read file: " + file_name);
				}
				
				//	Determine the file size and allocate an appropriate buffer
				f.seekg (0, f.end);
				long length = f.tellg();
				std::string data;
				data.resize(length);
				
				//	Read the whole file
				f.read(&data[0], length);
				
				return data;
			}
			
			//
			void writeFile(std::string file_name, std::string data, bool binary = false)
			{
				//
				auto flags = std::ofstream::out | std::ofstream::trunc;
				if ( binary == true ) {
					flags |= std::ofstream::binary;
				}
				
				//
				std::ofstream f(file_name, flags);
				
				//
				f.write(data.c_str(), data.size());
			}
			
		private:
			
			//
			Random random_;
	};
}




















#endif
