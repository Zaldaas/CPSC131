#ifndef PUHP_TESTS_TEST_HPP
#define PUHP_TESTS_TEST_HPP


//
#include "./deps/json.hpp"


//
#include "./ToString.hpp"
#include "./LogEntry.hpp"
#include "./Logs.hpp"
#include "./DeferredAssertion.hpp"


//
#include <memory>
#include <string>
#include <vector>


//
namespace PuhPTests
{
	/**
	 * This class represents one test case.
	 * A test case can contain multiple assertions and point adjustments; It's just a way to logically group things.
	*/
	class Test
	{
		//
		public:
			
			//
			static constexpr const char * DEFAULT_LABEL = "NOLABEL";
			
			///	Copy CTOR
			Test(const Test& other) {
				this->copyOther(other);
			}
			
			//
			std::string getLabel() const { return this->label_; }
			
			/**
			 * Grants points if the passed boolean evaluates to true
			 */
			void assertTrue(bool b, int points = 0, std::string label = Test::DEFAULT_LABEL) {
				
				//
				this->adjustPointsPossible(points, label);
				
				//
				this->deferredAssertions_.push_back(
					std::make_shared<DeferredAssertion>(
						label,
						points,
						AssertionType::True,
						b
					)
				);
			}
			
			/**
			 * Grants points if the passed boolean evaluates to false
			 */
			void assertFalse(bool b, int points = 0, std::string label = Test::DEFAULT_LABEL) {
				
				this->adjustPointsPossible(points, label);
				
				this->deferredAssertions_.push_back(
					std::make_shared<DeferredAssertion>(
						label,
						points,
						AssertionType::False,
						b
					)
				);
			}
			
			/**
			 * Grant points is the two items are "equal"
			 * 
			 * Equality is determined by the existing comparison operator between the two types,
			 * 	which you'll want to make sure are properly overloaded/defined.
			 * 
			 * Works with any two classes that have properly implemented comparison operators.
			 * 
			 * Will also attempt to print out the two items using to_string.
			 * Above, std::to_string is brought into the current namespace,
			 * 	which means you can overload simply "to_string" in this namespace to add support
			 * 	for additional types. See the overload for std::vector near the top of this source file.
			 */
			template<class T1, class T2>
			void assertEqual(T1 a, T2 b, int points = 0, std::string label = Test::DEFAULT_LABEL)
			{
				this->adjustPointsPossible(points, label);
				
				this->deferredAssertions_.push_back(
					std::make_shared<DeferredAssertion>(
						label,
						points,
						AssertionType::Equal,
						a, b
					)
				);
			}
			
			/**
			 * Opposite of assertEqual
			 */
			template<class T1, class T2>
			void assertNotEqual(T1 a, T2 b, int points = 0, std::string label = Test::DEFAULT_LABEL)
			{
				this->adjustPointsPossible(points, label);
				
				this->deferredAssertions_.push_back(
					std::make_shared<DeferredAssertion>(
						label,
						points,
						AssertionType::NotEqual,
						a, b
					)
				);
			}
			
			/**
			 * Attempts to run the passed lambda "func",
			 * and grants points if an exception DOES OCCUR.
			 */
			void assertException(std::function<void()> func, int points = 0, std::string label = Test::DEFAULT_LABEL) {
				
				this->adjustPointsPossible(points, label);
				
				this->deferredAssertions_.push_back(
					std::make_shared<DeferredAssertion>(
						label,
						points,
						AssertionType::Exception,
						func
					)
				);
			}
			
			/**
			 * Attempts to run the passed lambda "func",
			 * and grants points if an exception does not occur.
			 */
			void assertNoException(std::function<void()> func, int points = 0, std::string label = Test::DEFAULT_LABEL) {
				
				this->adjustPointsPossible(points, label);
				
				this->deferredAssertions_.push_back(
					std::make_shared<DeferredAssertion>(
						label,
						points,
						AssertionType::NoException,
						func
					)
				);
			}
			
			//
			bool getFailFast() { return this->fail_fast_; }
			void setFailFast(bool b) { this->fail_fast_ = b; }
			
			//
			bool run()
			{
				//
				bool any_failed = false;
				
				//
				for ( size_t i = 0; i < this->deferredAssertions_.size(); i++ ) {
					
					//
					auto ass = this->deferredAssertions_[i];
					
					//	Don't run anything twice
					if ( ass->hasRan() == true ) {
						continue;
					}
					
					//
					this->log(LogEntry(
						"Running: " + ass->getLabel(),
						LogEntryType::Info
					));
					
					//	Grab bool result of the lambda, BUT THEN explicitly set it to true or false
					//	Otherwise, a user function that forgot to return true/false could confuse the program
					bool passed = ass->run();
					if ( passed == true ) {
						passed = true;
					}
					else {
						passed = false;
					}
					
					//
					any_failed |= !passed;
					
					//
					std::string assert_message = ass->getMessage();
					std::string log_message = ass->getLabel() + " :: ";
					
					//
					if ( passed == true ) {
						log_message += "Award";
					}
					else {
						log_message += "Unable to award";
					}
					log_message += " " + std::to_string(ass->getPoints()) + " points";
					
					//
					if ( !assert_message.empty() ) {
						log_message += " :: " + assert_message;
					}
					
					//
					if ( passed == true ) {
						this->adjustPoints(
							ass->getPoints(),
							log_message,
							LogEntryType::Pass
						);
					}
					else {
						this->log(LogEntry(log_message, LogEntryType::Fail));
					}
					
					//	Fail fast?
					if ( this->fail_fast_ == true && passed == false ) {
						this->log(LogEntry(
							"Test failed and fail_fast_ is true; Abort remaining tests.",
							LogEntryType::Warning
						));
						break;
					}
				}
				
				//	Make sure all assertions are unmarked as ready to run
				//for ( DeferredAssertion& d : this->deferredAssertions_ ) {
				//	d.setReadyToRun(false);
				//}
				
				return !any_failed;
			}
			
			/// Write a log entry
			void log(const LogEntry& entry) {
				this->logs->log(entry);
			}
			/// Write a log entry
			void log(const std::string& s) {
				this->logs->log(s);
			}
			
			/**
			 * Adjusts points for this test, with a given reason and log entry type.
			 * Pass LogEntryType::None to let the method decide on Pass/Fail, based on the points.
			 * 
			 * You typically won't need to call this; It will be called by assertion methods above.
			 */
			int adjustPoints(int adjustment, std::string reason, LogEntryType log_type = LogEntryType::None) {
				
				//
				int points_old = this->points_;
				this->points_ += adjustment;
				
				//	Keep in bounds
				if ( this->points_ > this->getComputedPointsPossible() ) {
					
					//
					int points_oob = this->points_;
					this->points_ = this->getComputedPointsPossible();
					this->log(
						LogEntry(
							"Earned points (" + std::to_string(points_oob)+ ")"
							+ " have exceeded maximum possible points;"
							+ " Adjusting to stay within bounds: " + std::to_string(this->points_),
							LogEntryType::Info
						)
					);
				}
				
				//
				if ( log_type == LogEntryType::None) {
					if (adjustment > 0) {
						log_type = LogEntryType::Pass;
					}
					else if (adjustment < 0) {
						log_type = LogEntryType::Fail;
					}
				}
				this->log(
					LogEntry(
						"Points " + std::to_string(points_old) + " ==> " + std::to_string(this->points_) + " (" + std::to_string(adjustment) + "): " + reason,
						log_type
					)
				);
				
				return this->points_;
			}
			
			/**
			 * Adjust the total points possible.
			 * 
			 * Typically called by an assertion above, and does not need to be called by user code.
			 */
			int adjustPointsPossible(int points, std::string label) {
				
				//
				if ( this->isPointsPossibleFixed() == true ) {
					
					//
					//this->log(
					//	LogEntry(
					//		"Refusing to adjust points possible because points are already fixed to " + std::to_string(this->points_possible_fixed_)
					//		+ " (Label: " + label + ")",
					//		LogEntryType::Debug
					//	)
					//);
					
					return this->points_possible_fixed_;
				}
				
				//
				if (points > 0) {
					auto points_possible_old = this->points_possible_;
					this->points_possible_ += points;
					//this->log(label + " :: Adjusting points possible to " + std::to_string(this->points_possible_));
					this->log(
						"Adjusting points possible from " + std::to_string(points_possible_old)
						+ " to " + std::to_string(this->points_possible_)
						+ " (" + std::to_string(points)
						+ ") for label: "
						+ label
					);
				}
				
				return this->points_possible_;
			}
			
			//
			void setFixedPointsPossible(int points)
			{
				//
				this->points_possible_fixed_ = points;
				this->use_fixed_points_possible_ = true;
				
				this->log(
					LogEntry(
						"Setting fixed points possible to " + std::to_string(points),
						LogEntryType::Info
					)
				);
			}
			
			/// Return the number of points earned
			int getPoints() const { return this->points_; }
			
			///	Decide to normalize the final score/points to a target; Helps to weigh units
			void setNormalizedPointsPossibleTarget(int target)
			{
				this->points_possible_normalized_target_ = target;
				this->use_normalized_points_possible_ = true;
			}
			int getNormalizedPointsPossibleTarget()
			{
				//
				int points;
				
				//
				if ( this->isUsingNormalizedPoints() == true ) {
					points = this->points_possible_normalized_target_;
				}
				else {
					points = this->getComputedPointsPossible();
				}
				
				return points;
			}
			bool isUsingNormalizedPoints() { return this->use_normalized_points_possible_; }
			
			///	Compute a points value that is normalized to a target value; Useful for weighting units
			int computeNormalizedPoints()
			{
				return this->computeNormalizedPoints(this->points_possible_normalized_target_);
			}
			int computeNormalizedPoints(int target)
			{
				//
				if ( this->isUsingNormalizedPoints() == false ) {
					return this->getPoints();
				}
				
				//
				if ( target == 0 ) {
					throw std::runtime_error("Cannot normalize to a target of 0");
				}
				
				//
				double d = static_cast<double>(this->points_) / static_cast<double>(this->getComputedPointsPossible());
				
				//
				double dNormed = round(target * d);
				double iNormed = static_cast<int>(dNormed);
				
				return iNormed;
			}
			
			/// Return the number of points possible
			int getDynamicPointsPossible() const { return this->points_possible_; }
			
			///	Get the fixed points possible; Does not indicate whether fixed points possible is enabled.
			int getFixedPointsPossible() { return this->points_possible_fixed_; }
			
			///	Returns true if the points possible is fixed
			bool isPointsPossibleFixed() { return this->use_fixed_points_possible_; }
			
			///	Returns the number of points possible, whether dynamic or fixed
			int getComputedPointsPossible()
			{
				//
				int points = this->points_possible_;
				
				//
				if ( this->isPointsPossibleFixed() == true ) {
					points = this->getFixedPointsPossible();
				}
				
				return points;
			}
			
			///	Wrapper
			std::string getPassFailLogsAsString(size_t label_room = 0) { return this->logs->getPassFailEntriesAsString(label_room); }
			
			/// Render this entire test case to a JSON object
			nlohmann::json toJson() {
				return nlohmann::json {
					{"label", this->label_},
					{"logs", this->logs->getEntriesAsString() },
					{"points", this->points_},
					{"points_possible", this->points_possible_}
				};
			}
			
			/// Render this entire test case to a JSON object, compatible with Gradescope
			nlohmann::json toGradescopeJson() {
				return nlohmann::json {
					{"score", this->isUsingNormalizedPoints() ? this->computeNormalizedPoints() : this->getPoints()},
					{"max_score", isUsingNormalizedPoints() ? this->getNormalizedPointsPossibleTarget() : this->getComputedPointsPossible()},
					{"output", this->logs->getEntriesAsString()},
					{"name", this->label_}
				};
			}
			
			/// Assignment operator overload
			Test& operator=(const Test& rhs) {
				return this->copyOther(rhs);
			}
			
		private:
			
			//
			std::string label_;
			int points_;
			int points_possible_;
			
			int points_possible_fixed_ = 0;
			bool use_fixed_points_possible_ = false;
			
			int points_possible_normalized_target_ = 0;
			bool use_normalized_points_possible_ = false;
			
			/// Whether to fail the whole test on the first failed assertion
			bool fail_fast_ = true;
			
			/// Vector of assertions to run, when the test is run
			std::vector<std::shared_ptr<DeferredAssertion>> deferredAssertions_;
			
			///	Logs class
			std::unique_ptr<Logs> logs;
			
			/// Private CTOR to ensure only the Tests class can create an instance
			Test(std::string label, int points = 0, int points_possible = 0)
				: label_(label), points_(points), points_possible_(points_possible)
			{
				//
				this->logs = std::unique_ptr<Logs>(new Logs(this->label_));
			}
			
			/// Helper function to make this test case into a copy of another.
			Test& copyOther(const Test& other)
			{
				this->label_ = other.label_;
				
				this->points_ = other.points_;
				this->points_possible_ = other.points_possible_;
				this->points_possible_fixed_ = other.points_possible_fixed_;
				this->use_fixed_points_possible_ = other.use_fixed_points_possible_;
				this->points_possible_normalized_target_ = other.points_possible_normalized_target_;
				this->use_normalized_points_possible_ = other.use_normalized_points_possible_;
				
				this->fail_fast_ = other.fail_fast_;
				
				//
				this->deferredAssertions_.clear();
				for ( auto d_other : other.deferredAssertions_) {
					auto d = std::make_shared<DeferredAssertion>(*d_other);
					this->deferredAssertions_.push_back(d);
				}
				
				this->logs = std::unique_ptr<Logs>(new Logs(*other.logs));
				
				return *this;
			}
			
			///	Declare Tests is a friend
			friend class Tests;
	};
}













#endif
