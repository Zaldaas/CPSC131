#ifndef PUHP_TESTS_LEADERBOARD_HPP
#define PUHP_TESTS_LEADERBOARD_HPP


//
#include "./deps/json.hpp"


//
#include "./Enums.hpp"
#include "./LeaderboardEntry.hpp"


//
#include <map>
#include <stdexcept>
#include <string>


//
namespace PuhPTests
{
	//
	template<class ENTRY_VTYPE>
	class Leaderboard
	{
		//
		public:
			
			//
			Leaderboard(const Leaderboard<ENTRY_VTYPE>& other)
			{
				this->copyOther(other);
			}
			
			//
			void addEntry(std::string name, ENTRY_VTYPE value)
			{
				LeaderboardEntry<ENTRY_VTYPE> entry(name, value);
				this->addEntry(entry);
			}
			
			//
			void addEntry(const LeaderboardEntry<ENTRY_VTYPE>& entry)
			{
				//
				this->entries_[entry.getName()] = entry;
				
				//
				if ( this->sort_key_.empty() ) {
					this->sort_key_ = entry.getName();
				}
			}
			
			//
			void setSortKey(std::string k) { this->sort_key_ = k; }
			std::string getSortKey() const { return this->sort_key_; }
			
			//
			static std::string sortDirectionToString(LeaderboardSortDirection d)
			{
				switch(d)
				{
					case LeaderboardSortDirection::Ascending:
						return "Ascending";
					
					case LeaderboardSortDirection::Default:
						return "Default";
					
					case LeaderboardSortDirection::Descending:
						return "Descending";
				}
				
				throw std::runtime_error("Unsupported Leaderboard sort direction");
			}
			static std::string sortDirectionToGradescopeString(LeaderboardSortDirection d)
			{
				switch(d)
				{
					case LeaderboardSortDirection::Ascending:
						return "asc";
					
					case LeaderboardSortDirection::Default:
						return "desc";
					
					case LeaderboardSortDirection::Descending:
						return "desc";
				}
				
				throw std::runtime_error("Unsupported Leaderboard sort direction");
			}
			
			//
			void setSortDirection(LeaderboardSortDirection d)
			{
				switch(d)
				{
					case LeaderboardSortDirection::Ascending:
					case LeaderboardSortDirection::Default:
					case LeaderboardSortDirection::Descending:
						this->sort_direction_ = d;
						return;
				}
				
				throw std::runtime_error("Unsupported Leaderboard sort direction: " + Leaderboard::sortDirectionToString(d));
			}
			LeaderboardSortDirection getSortDirection() const { return this->sort_direction_; }
			std::string getSortDirectionAsString() const
			{
				return Leaderboard::sortDirectionToString( this->getSortDirection() );
			}
			std::string getSortDirectionAsGradescopeString() const
			{
				return Leaderboard::sortDirectionToGradescopeString( this->getSortDirection() );
			}
			
			//
			nlohmann::json toGradescopeJson() const
			{
				//
				nlohmann::json j;
				
				//
				bool found_sort_key = false;
				for ( auto itr : this->entries_ ) {
					
					//
					std::string name = itr.first;
					LeaderboardEntry<ENTRY_VTYPE> entry = itr.second;
					
					//
					auto jj = entry.toGradescopeJson();
					
					//
					if ( entry.getName() == this->sort_key_ ) {
						
						//
						found_sort_key = true;
						
						//
						jj["order"] = this->getSortDirectionAsGradescopeString();
					}
					
					j.push_back(jj);
				}
				
				//
				if ( found_sort_key == false ) {
					throw std::runtime_error("Did not find sort key while rendering Leaderboard entries");
				}
				
				return j;
			}
			
			//
			Leaderboard<ENTRY_VTYPE>& operator=(const Leaderboard<ENTRY_VTYPE>& other)
			{
				return this->copyOther(other);
			}
			
			///	Declare that Tests is a friend
			friend class Tests;			
			
		//
		private:
			
			//	Prevent direct instancing
			//	(use Tests::createLeaderboard instead)
			Leaderboard() {}
			
			//
			std::map<std::string, LeaderboardEntry<ENTRY_VTYPE>> entries_;
			
			std::string sort_key_;
			LeaderboardSortDirection sort_direction_ = LeaderboardSortDirection::Default;
			
			//
			Leaderboard<ENTRY_VTYPE>& copyOther(const Leaderboard<ENTRY_VTYPE>& other)
			{
				//
				this->entries_ = other.entries_;
				this->sort_key_ = other.sort_key_;
				this->sort_direction_ = other.sort_direction_;
				
				return *this;
			}
	};
}








#endif
