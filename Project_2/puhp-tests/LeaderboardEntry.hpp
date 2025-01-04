#ifndef PUHP_TESTS_LEADERBOARD_ENTRY_HPP
#define PUHP_TESTS_LEADERBOARD_ENTRY_HPP


//
#include "./deps/json.hpp"


//
#include "./Enums.hpp"


//
#include <stdexcept>
#include <string>


//
namespace PuhPTests
{
	//
	template<class VTYPE>
	class LeaderboardEntry
	{
		//
		public:
			
			//
			LeaderboardEntry() {}
			
			//
			LeaderboardEntry(std::string name, VTYPE value)
			{
				//
				this->setName(name);
				this->setValue(value);
			}
			
			//
			void setName(std::string name) { this->name_ = name; }
			std::string getName() const { return this->name_; }
			
			void setValue(VTYPE value) { this->value_ = value; }
			VTYPE getValue() const { return this->value_; }
			
			//
			nlohmann::json toGradescopeJson() const
			{
				//
				nlohmann::json j;
				
				//
				j["name"] = this->getName();
				j["value"] = this->getValue();
				
				return j;
			}
			
		//
		private:
			
			//
			std::string name_;
			VTYPE value_;
	};
}








#endif
