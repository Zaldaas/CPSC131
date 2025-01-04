#ifndef PUHP_TESTS_RAII_PIPE_HPP
#define PUHP_TESTS_RAII_PIPE_HPP


//
//#include <stdio.h>
//#include <stdlib.h>
#include <cstring>
#include <exception>
#include <memory>
#include <stdexcept>
#include <unistd.h>
#include <vector>


/**
 * TODO: I dunno.
 * Maybe add a mutex around race conditions here, even though I'll probably never use this in a multi-threaded app
 */
namespace PuhPTests
{
	
	//
	class RAII_Pipe
	{
		//
		public:
			
			//
			static const int PIPE_READ_END = 0;
			static const int PIPE_WRITE_END = 1;
			
			//
			static const int FD_STDIN = 0;
			static const int FD_STDOUT = 1;
			static const int FD_STDERR = 2;
			
			//
			static const size_t BUFFER_SIZE = 4096;
			
			//
			RAII_Pipe()
			{
				if ( pipe(this->pipe_) == -1 ) {
					throw std::runtime_error(std::string("RAII_Pipe() - Unable to make pipe: ") + strerror(errno));
				}
			}
			
			//
			~RAII_Pipe()
			{
				this->close();
			}
			
			///	Close both ends of the pipe
			void close()
			{
				this->closeRead();
				this->closeWrite();
			}
			
			//
			void closeRead()
			{
				this->closeOne(this->pipe_[RAII_Pipe::PIPE_READ_END]);
			}
			
			//
			void closeWrite()
			{
				this->closeOne(this->pipe_[RAII_Pipe::PIPE_WRITE_END]);
			}
			
			int getReadEnd() { return this->pipe_[RAII_Pipe::PIPE_READ_END]; }
			int getWriteEnd() { return this->pipe_[RAII_Pipe::PIPE_WRITE_END]; }
			
			///	Redirect this pipe so it reads from stdin
			void redirect_readFromStdin()
			{
				this->duplicateToFileDescriptor(
					this->pipe_[RAII_Pipe::PIPE_READ_END],
					RAII_Pipe::FD_STDIN
				);
			}
			///	Redirect this pipe so it writes to stdout
			void redirect_writeToStdout()
			{
				this->duplicateToFileDescriptor(
					this->pipe_[RAII_Pipe::PIPE_WRITE_END],
					RAII_Pipe::FD_STDOUT
				);
			}
			///	Redirect this pipe so it writes to stderr
			void redirect_writeToStderr()
			{
				this->duplicateToFileDescriptor(
					this->pipe_[RAII_Pipe::PIPE_WRITE_END],
					RAII_Pipe::FD_STDERR
				);
			}
			
			///	Write some data to the pipe
			void write(const char * data)
			{
				this->write(std::string(data));
			}
			void write(const std::string& data)
			{
				//
				if ( data.empty() ) {
					return;
				}
				
				//	Write the data, piece by piece
				ssize_t sz = static_cast<ssize_t>(data.size());
				ssize_t pos = 0;
				int fd = this->getWriteEnd();
				while ( pos < sz )
				{
					//
					ssize_t bytesToWrite = sz - pos;
					ssize_t bytesWritten = ::write(fd, &data[pos], bytesToWrite);
					if ( bytesWritten < 0 ) {
						throw std::runtime_error(
							"RAII_Pipe() - Failed to write " + std::to_string(bytesToWrite)
							+ " bytes to pipe " + std::to_string(fd)
							+ ": " + strerror(errno)
						);
					}
					
					//
					pos += bytesWritten;
				}
			}
			
			///	Read some data from the pipe
			///	TODO: Maybe add an argument to limit the max bytes to read at one time
			std::string read()
			{
				//
				std::string returnValue;
				char buffer[RAII_Pipe::BUFFER_SIZE];
				std::vector<std::pair<std::shared_ptr<char[]>, size_t>> chunks;
				int fd = this->getReadEnd();
				
				//
				size_t totalBytesRead = 0;
				while ( true )
				{
					//	Read some bytes
					size_t bytesRead = ::read(fd, buffer, RAII_Pipe::BUFFER_SIZE);
					if ( bytesRead < 0 ) {
						throw std::runtime_error(std::string("Process::readPipe() - Error while reading pipe: ") + strerror(errno));
					}
					else if ( bytesRead == 0 ) {
						break;
					}
					
					//	Make a new chunk and give it the data
					std::pair<std::shared_ptr<char[]>, size_t> to_add;
					char * chunk = new char[bytesRead];
					strncpy(chunk, buffer, bytesRead);
					to_add.first = std::shared_ptr<char[]>(chunk);
					to_add.second = bytesRead;
					chunks.push_back(to_add);
					
					//	Keep track of total bytes read
					totalBytesRead += bytesRead;
				}
				
				//	Combine the chunks.
				//	Yes, I realize this is inefficient, but I am emotionally attached to std::string
				size_t writePosition = 0;
				returnValue.resize(totalBytesRead + 1);
				for ( auto p : chunks ) {
					
					//
					auto pChunk = p.first;
					size_t chunkSize = p.second;
					
					//
					strncpy(&returnValue[writePosition], pChunk.get(), chunkSize);
					writePosition += chunkSize;
				}
				chunks.clear();
				
				return returnValue;
			}
		
		//
		private:
			
			//
			int pipe_[2];
			
			//
			void closeOne(int& pipe_end)
			{
				if ( pipe_end >= 0 ) {
					::close(pipe_end);
					pipe_end = -1;
				}
			}
			
			/**
			 * Redirect an existing file descriptor to another.
			 * Useful to make this pipe read or write to some standard input/output.
			 * Will close the original file descriptor upon success.
			 */
			void duplicateToFileDescriptor(int& current_fd, int target_fd, bool force = false)
			{
				//	Don't do anything if not needed, and not forced
				if ( force == false && current_fd == target_fd ) {
					return;
				}
				
				//
				int original_fd = current_fd;
				
				//
				if ( dup2(current_fd, target_fd) != target_fd ) {
					throw std::runtime_error(
						"RAII_Pipe::duplicateToFileDescriptor() - Unable to duplicate fd " + std::to_string(current_fd)
						+ " to " + std::to_string(target_fd)
						+ ": " + strerror(errno)
					);
				}
				
				//	Close the original fd number
				::close(original_fd);
			}
	};
}





#endif

