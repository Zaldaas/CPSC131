#ifndef PUHP_TESTS_LOGENTRY_HPP
#define PUHP_TESTS_LOGENTRY_HPP


//
#include "./Enums.hpp"


//
#include <stdexcept>
#include <string>


//
namespace PuhPTests
{
	
	/**
	 * Class to hold a single log entry.
	 */
	class LogEntry
	{
		//
		public:
		
			LogEntry(std::string message, LogEntryType type = LogEntryType::Info)
				: message_(message), type_(type)
			{}
			
			LogEntryType getType() const {
				return this->type_;
			}
			
			/**
			 * Render this log entry as a string appropriate for printing.
			 */
			std::string toString() const {
				
				std::string tString = LogEntry::typeToString(this->type_);
				if ( this->type_ == LogEntryType::Error || this->type_ == LogEntryType::Fail ) {
					tString = "***" + tString + "***";
				}
				
				return "[" + tString + "] " + this->message_;
			}
			
			/**
			 * Convert a LogEntryType enumeration value to a string, to aid printing.
			 */
			static std::string typeToString(LogEntryType t) {
				switch(t)
				{
					case LogEntryType::None:
						return "None";
					case LogEntryType::Any:
						return "Any";
					case LogEntryType::Debug:
						return "Debug";
					case LogEntryType::Info:
						return "Info";
					case LogEntryType::Pass:
						return "Pass";
					case LogEntryType::Fail:
						return "Fail";
					case LogEntryType::Warning:
						return "Warning";
					case LogEntryType::Error:
						return "Error";
				}
				throw std::logic_error("Somehow made it to the end of the switch in PuhPTests::LogEntry::typeToString");
			}
			
		private:
			std::string message_;
			LogEntryType type_;
	};
}


#endif
