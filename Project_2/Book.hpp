#ifndef BOOK_HPP
#define BOOK_HPP


/**
 * You do not need to edit this file,
 * 	though you can if you wish to add additional helper methods
 */


//
#include <string>


//
namespace CPSC131::BookStore
{
	class Book
	{
		//
		public:
			
			//	CTORS
			Book();
			Book(std::string title, std::string author, std::string isbn, size_t price_cents = 0, size_t stock_available = 0);
			
			//
			std::string getTitle() const;
			std::string getAuthor() const;
			std::string getIsbn() const;
			size_t getPriceCents() const;
			
			void adjustStockAvailable(int adjustment);
			size_t getStockAvailable() const;
			
			//
			bool operator ==(const Book& other) const;
			bool operator !=(const Book& other) const;
			
			
		//
		private:
			
			//
			std::string
				title_ = "",
				author_ = "",
				isbn_ = ""
				;
			size_t price_cents_ = 0;
			size_t stock_available_ = 0;
	};
}


#endif
