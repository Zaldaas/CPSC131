#ifndef PUHP_TESTS_TESTS_HPP
#define PUHP_TESTS_TESTS_HPP


//
#include "./deps/json.hpp"


//
#include "./Test.hpp"
#include "./Leaderboard.hpp"


//
#include <cmath>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>


//
namespace PuhPTests
{
	//
	class Tests
	{
		public:
			
			//
			typedef double LEADERBOARD_VTYPE;
			
			//
			static constexpr int DEFAULT_INDENT = 3;
			static constexpr const char * DEFAULT_RESULTS_FILENAME = "results.json";
			static const int DEFAULT_NORMALIZED_POINTS_TARGET = 100;
			
			/// CTOR
			Tests(int points_normalized_target = Tests::DEFAULT_NORMALIZED_POINTS_TARGET)
			{
				this->points_normalized_target_ = points_normalized_target;
				this->logs = std::unique_ptr<Logs>(new Logs("Tests Container"));
			}
			
			///	Set the fail-fast variable
			void setFailFast(bool b)
			{
				this->fail_fast_ = b;
			}
			
			///	Create a new Leaderboard
			std::shared_ptr<Leaderboard<LEADERBOARD_VTYPE>> createLeaderboard(std::string sort_key = "", bool set = true)
			{
				//	Create the object
				auto b = std::shared_ptr<Leaderboard<LEADERBOARD_VTYPE>>(new Leaderboard<LEADERBOARD_VTYPE>());
				
				//	Set sort key
				b->setSortKey(sort_key);
				
				//	Add to myself?
				if ( set == true ) {
					this->setLeaderboard(b);
				}
				
				return b;
			}
			
			///	Create a new test
			std::shared_ptr<Test> createTest(std::string label, int points = 0, int points_possible = 0, bool add = true)
			{
				//	Create the object
				auto t = std::shared_ptr<Test>(new Test(label, points, points_possible));
				
				//	Add to myself?
				if ( add == true ) {
					this->add(t);
				}
				
				return t;
			}
			
			/// Add a test to this group of tests
			void add(std::shared_ptr<Test> test)
			{
				this->tests_.push_back(test);
				this->longest_test_label_ = std::max(this->longest_test_label_, test->getLabel().size());
			}
			
			///	Run all tests
			void run(bool finish = true)
			{
				//
				this->logs->log("Begin running tests");
				
				//
				for ( size_t i = 0; i < this->tests_.size(); i++ ) {
					
					//
					auto t = this->tests_[i];
					
					//
					this->logs->log("Running test: " + t->getLabel());
					bool result = t->run();
					
					//	Fail fast?
					if ( result == false && this->fail_fast_ == true ) {
						this->logs->log(
							LogEntry(
								"Test \"" + t->getLabel() + "\" failed and fail-fast mode is enabled; Abort remaining tests.",
								LogEntryType::Warning
							)
						);
						break;
					}
				}
				this->logs->log("Finished running tests");
				
				//
				this->calculateTotals();
				
				//
				if ( finish == true ) {
					std::cout << std::endl;
					this->finish();
				}
			}
			
			///	Output tests container logs, output pass/fail logs, write for gradescope
			void finish(bool gradescope = true)
			{
				this->printPassFailLogsAsString();
				this->printTestsSummary();
				
				//
				if ( gradescope == true ) {
					this->writeResultsAsGradescopeJson();
				}
			}
			
			///	Set the Leaderboard object
			void setLeaderboard(std::shared_ptr<Leaderboard<LEADERBOARD_VTYPE>> board)
			{
				this->leaderboard_ = board;
			}
			
			///	Clear/remove the Leaderboard object
			void clearLeaderboard()
			{
				this->leaderboard_ = nullptr;
			}
			
			/// Write a log entry
			void log(const LogEntry& entry)
			{
				this->logs->log(entry);
			}
			/// Write a log entry
			void log(const std::string& s)
			{
				this->logs->log(s);
			}
			
			/**
			 * Set to true if you want gradescope to show stdout to students
			 * If this is set to false, it's likely some students will see
			 * a blank page when they submit code that doesn't compile.
			 */
			void setGradescopeShowStdoutToStudents(bool b)
			{
				this->gradescope_show_stdout_to_students_ = b;
			} 
			
			/**
			 * Render a JSON object that contains each test's properties.
			 */
			nlohmann::json getTestsAsJson() const
			{
				//
				nlohmann::json j;
				for ( std::shared_ptr<Test> tt : this->tests_) {
					j.push_back(tt->toJson());
				}
				
				return j;
			}
			/**
			 * Render a JSON object in a format compatible with Gradescope,
			 * that contains the results of all tests.
			 */
			nlohmann::json getTestsAsGradescopeJson() const
			{
				//
				nlohmann::json j;
				for ( std::shared_ptr<Test> tt : this->tests_) {
					j.push_back(tt->toGradescopeJson());
				}
				
				return j;
			}
			
			/**
			 * Render the results of all tests, along with a test summary, to a JSON object
			 */
			nlohmann::json getResultsAsJson() const
			{
				//
				nlohmann::json results;
				
				//
				results["output"] = this->getTestsSummaryAsString();
				results["tests"] = this->getTestsAsJson();
				
				return results;
			}
			
			/**
			 * Render a JSON object containing all tests and information needed for Gradescope.
			 */
			nlohmann::json getResultsAsGradescopeJson() const
			{
				//
				nlohmann::json results;
				
				//
				results["score"] = this->points_normalized_;
				results["output"] = this->getTestsSummaryAsString();
				results["tests"] = this->getTestsAsGradescopeJson();
				
				//
				if ( this->gradescope_show_stdout_to_students_ == true ) {
					results["stdout_visibility"] = "visible";
				}
				else {
					results["stdout_visibility"] = "hidden";
				}
				
				//
				if ( this->leaderboard_ != nullptr ) {
					results["leaderboard"] = this->leaderboard_->toGradescopeJson();
				}
				
				return results;
			}
			
			/// Render all test results to a string.
			std::string getResultsAsString()
			{
				//
				nlohmann::json results;
				
				//
				results["tests"] = this->getTestsAsJson();
				
				return results.dump(Tests::DEFAULT_INDENT);
			}
			
			/**
			 * Write the results (output, tests) to a file, in JSON format
			 */
			void writeResultsAsJson(std::string file_path = Tests::DEFAULT_RESULTS_FILENAME ) const
			{
				//
				nlohmann::json results = this->getResultsAsJson();
				
				//
				{
					std::ofstream f(file_path);
					f << results.dump(Tests::DEFAULT_INDENT);
				}
			}
			
			/**
			 * Write the results (output, tests) to a file, in a JSON format compatible with Gradescope
			 * This is typically the method you'll want to call after all tests, inside your auto grader.
			 */
			void writeResultsAsGradescopeJson(std::string file_path = Tests::DEFAULT_RESULTS_FILENAME ) const
			{
				//	Remove the results file first
				{
					FILE* file;
					if ( (file = fopen(file_path.c_str(), "r")) ) {
						fclose(file);
						if ( remove(file_path.c_str()) != 0 ) {
							throw std::runtime_error("Unable to remove gradescope " + file_path + " file before writing.");
						}
					}
				}
				
				//
				nlohmann::json results = this->getResultsAsGradescopeJson();
				
				//
				{
					std::ofstream f(file_path);
					f << results.dump(Tests::DEFAULT_INDENT);
				}
			}
			
			/**
			 * Print a summary of tests to stdout.
			 * This is useful to help the student get a general idea of where they need to improve.
			 */
			void printTestsSummary()
			{
				std::cout << this->getTestsSummaryAsString() << std::endl;
			}
			
			/**
			 * Generate a string summary of the tests.
			 * This is useful to help the student get a general idea of where they need to improve.
			 */
			std::string getTestsSummaryAsString() const
			{
				//
				std::stringstream ss;
				
				//
				ss << "*************************" << std::endl;
				ss << "***** Tests Summary *****" << std::endl;
				
				//
				for ( std::shared_ptr<Test> t : this->tests_ ) {
					
					//
					ss
						<< "[" << t->getLabel()<< "] "
						<< t->getPoints() << " / " << t->getComputedPointsPossible()
						;
					
					//
					if ( t->isUsingNormalizedPoints() == true ) {
						ss
							<< " ==> "
							<< t->computeNormalizedPoints() << " / " << t->getNormalizedPointsPossibleTarget()
							;
					}
					
					//
					ss << std::endl;
				}
				
				//
				ss << std::endl;
				ss << "Raw Total: " << this->points_total_ << " / " << this->points_possible_ << " (" << this->points_percent_earned_ << "%)" << std::endl;
				ss << "Normalized: " << this->points_normalized_ << " / " << this->points_normalized_target_ << std::endl;
				
				return ss.str();
			}
			
			/**
			 * Generate a summary of logs, but ONLY logs of type PASS or FAIL
			 * This can be useful to help the student ignore log entries that
			 * 	don't affect their score.
			 */
			std::string getPassFailLogsAsString() const
			{
				std::stringstream ss;
				
				ss << "****************************" << std::endl;
				ss << "***** Pass / Fail Logs *****" << std::endl;
				
				for ( auto t : this->tests_ ) {
					ss << t->getPassFailLogsAsString(this->longest_test_label_);
				}
				return ss.str();
			}
			void printPassFailLogsAsString() const
			{
				std::cout << this->getPassFailLogsAsString() << std::endl;
			}
			
			/**
			 * Assignment operator
			 */
			Tests& operator=(const Tests& other)
			{
				return this->copyOther(other);
			}
			
			/**
			 * Overload the stream operator to do appending
			 * Allows you to do something like:
			 * 
			 * Tests tests;
			 * Test t("My test");
			 * tests << t;
			 * 
			 * Alternatively, just use the add() method.
			 */
			Tests& operator<<(const std::shared_ptr<Test>& t)
			{
				this->add(t);
				
				return *this;
			}
		
		private:
			
			///	Raw total points for all tests
			int points_total_ = 0;
			
			///	Raw total points possible
			int points_possible_ = 0;
			
			///	Ratio of points possible earned
			double points_ratio_earned_ = 0.0;
			
			///	Percentage of points possible earned
			double points_percent_earned_ = 0.0;
			
			/**
			 * Normalization target for the overall test suite
			 * If 0, no normalization will be calculated
			 */
			int points_normalized_target_ = 0;
			
			/// Calculated normalized points, overall
			int points_normalized_ = 0;
			
			/// Tests contained by this object
			std::vector<std::shared_ptr<Test>> tests_;
			
			///	Leaderboard, if any
			std::shared_ptr<Leaderboard<LEADERBOARD_VTYPE>> leaderboard_ = nullptr;
			
			/// Track the longest test label, to make output a little more beautiful
			size_t longest_test_label_ = 0;
			
			/// Whether to fail all tests on the first test that failed anything
			bool fail_fast_ = true;
			
			///	Whether to instruct Gradescope to show stdout to students
			bool gradescope_show_stdout_to_students_ = true;
			
			///	Log entries specific to this tests container
			std::unique_ptr<Logs> logs;
			
			///	Calculate normalized points
			void calculateTotals()
			{
				//
				int
					total = 0,
					total_possible = 0,
					total_normalized = 0
					;
				
				//
				for ( std::shared_ptr<Test> t : this->tests_ ) {
					
					//
					total += t->computeNormalizedPoints();
					total_possible += t->getNormalizedPointsPossibleTarget();
				}
				
				//
				if ( total_possible != 0 ) {
					this->points_ratio_earned_ = (
						static_cast<double>(total) / static_cast<double>(total_possible)
					);
				}
				else {
					this->points_ratio_earned_ = 0.0;
				}
				this->points_percent_earned_ = round(
					(this->points_ratio_earned_ * 10000) / 100.0
				);
				
				// If target is 0, we don't normalize; Just copy over.
				if ( this->points_normalized_target_ == 0 || total_possible == 0 ) {
					total_normalized = total;
				}
				
				//	Otherwise, attempt to normalize the points
				else {
					total_normalized = static_cast<int>(
						round(
							static_cast<double>(this->points_normalized_target_)
							*
							this->points_ratio_earned_
						)
					);
				}
				
				//	Assign internally
				this->points_total_ = total;
				this->points_possible_ = total_possible;
				this->points_normalized_ = total_normalized;
			}
			
			/**
			 * Copy another Tests object
			 */
			Tests& copyOther(const Tests& other)
			{
				//
				this->points_total_ = other.points_total_;
				this->points_possible_ = other.points_possible_;
				this->points_ratio_earned_ = other.points_ratio_earned_;
				this->points_percent_earned_ = other.points_percent_earned_;
				this->points_normalized_target_ = other.points_normalized_target_;
				this->points_normalized_ = other.points_normalized_;
				
				//	Make copies of internal tests
				//	(don't just steal pointers)
				this->tests_.clear();
				for ( std::shared_ptr<Test> t_other : other.tests_ ) {
					auto t = std::make_shared<Test>(*t_other);
					this->tests_.push_back(t);
				}
				
				//
				this->longest_test_label_ = other.longest_test_label_;
				
				this->gradescope_show_stdout_to_students_ = other.gradescope_show_stdout_to_students_;
				
				this->logs = std::unique_ptr<Logs>(new Logs(*other.logs));
				
				return *this;
			}
	};
}















#endif
