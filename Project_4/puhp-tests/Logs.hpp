#ifndef PUHP_TESTS_LOGS_HPP
#define PUHP_TESTS_LOGS_HPP


//
#include "./deps/json.hpp"


//
#include "./Enums.hpp"
#include "./LogEntry.hpp"


//
#include <iostream>
#include <string>
#include <vector>


//
namespace PuhPTests
{
	//
	class Logs
	{
		//
		public:
			
			///	Basic CTOR
			Logs(std::string label)
			{
				this->label_ = label;
			}
			
			///	Copy CTOR
			Logs(const Logs& other)
			{
				this->copyOther(other);
			}
			
			/// Write a log entry
			void log(const LogEntry& entry)
			{
				this->entries_.push_back(entry);
				std::cout << "[" << this->label_ << "]" << entry.toString() << std::endl;
			}
			
			/// Write a log entry
			void log(const std::string& s)
			{
				LogEntry entry(s, LogEntryType::Info);
				this->log(entry);
			}
			
			/// Render all log entries to a newline-separated string. Useful for printing.
			std::string getEntriesAsString(std::vector<LogEntryType> log_types = {LogEntryType::Any}, size_t label_room = 0)
			{
				//
				std::stringstream ss;
				
				//
				std::string label = this->label_;
				size_t label_padding = 0;
				if ( label.size() < label_room ) {
					label_padding = label_room - label.size();
				}
				
				//
				bool any = std::find(log_types.begin(), log_types.end(), LogEntryType::Any) != log_types.end();
				for ( const LogEntry& entry : this->entries_ ) {
					
					LogEntryType t = entry.getType();
					
					//	Print if we're printing all, or this is the right type of log
					if ( any || std::find(log_types.begin(), log_types.end(), t) != log_types.end() ) {
						
						//	Print the log line
						ss << "[" << this->label_ << std::string(label_padding, ' ') << "]" << entry.toString() << std::endl;
					}
				}
				
				return ss.str();
			}
			
			/// Render all logs *of a particular type* to a newline-separated string. Useful for printing.
			std::string getEntriesAsString(LogEntryType t, size_t label_room = 0)
			{
				return this->getEntriesAsString({t}, label_room);
			}
			/// Render all Pass/Fail logs to a newline-separated string. Useful for printing to students.
			std::string getPassFailEntriesAsString(size_t label_room = 0)
			{
				return this->getEntriesAsString({LogEntryType::Pass, LogEntryType::Fail}, label_room);
			}
			
			/// Render all logs to a JSON object
			nlohmann::json getEntriesAsJson()
			{
				//
				nlohmann::json j;
				
				//
				for ( const LogEntry& entry : this->entries_ ) {
					j.push_back(entry.toString());
				}
				
				return j;
			}
			
		//
		private:
			
			///	Label
			std::string label_;
			
			///	Log entries
			std::vector<LogEntry> entries_;
			
			Logs& copyOther(const Logs& other)
			{
				this->label_ = other.label_;
				this->entries_ = other.entries_;
				
				return *this;
			}
	};
}








#endif
