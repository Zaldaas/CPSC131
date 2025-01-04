#ifndef PUHP_TESTS_ENUMS_HPP
#define PUHP_TESTS_ENUMS_HPP

//
namespace PuhPTests
{
	/**
	 * Types of assertions
	 */
	enum class AssertionType
	{
		True,
		False,
		Equal,
		NotEqual,
		Exception,
		NoException
	};
	
	
	/**
	 * Types of log entries
	 */
	enum class LogEntryType
	{
		None,
		Any,
		Debug,
		Info,
		Warning,
		Error,
		Pass,
		Fail,
	};
	
	/**
	 * Sort directions for the Leaderboard
	 */
	enum class LeaderboardSortDirection
	{
		Default,
		Ascending,
		Descending
	};
}

#endif

