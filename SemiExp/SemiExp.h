#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Darshan Masti Prakash, Syracuse University,          //
//				dmastipr@syr.edu									 //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
*
* Build Process:
* --------------
* Required Files:
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
*
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.3 : 09 Feb 2016
- Implemented ITokenCollection Interface and added the definitions for those methods
* ver 3.2 : 02 Feb 2016
* - declared SemiExp copy constructor and assignment operator = delete
* - added default argument for Toker pointer to nullptr so SemiExp
*   can be used like a container of tokens.
* - if pToker is nullptr then get() will throw logic_error exception
* ver 3.1 : 30 Jan 2016
* - changed namespace to Scanner
* - fixed bug in termination due to continually trying to read
*   past end of stream if last tokens didn't have a semiExp termination
*   character
* ver 3.0 : 29 Jan 2016
* - built in help session, Friday afternoon
*
* Planned Additions and Changes:
* ------------------------------
* - add public :, protected :, private : as terminators
* - move creation of tokenizer into semiExp constructor so
*   client doesn't have to write that code.
*/

#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/ITokCollection.h"

namespace Scanner
{
	using Token = std::string;

	//Implemented Interface ITokCollection and inheriting it 
	class SemiExp : public ITokCollection
	{
	public:
		SemiExp(Toker* pToker = nullptr);
		SemiExp(const SemiExp&) = delete;
		SemiExp& operator=(const SemiExp&) = delete;
		size_t length();
		bool get(bool clear = true); // Get functionality which handles the token get
		bool getSemiExpHelper(std::string token, std::string &tempSemiExp);
		bool getHelperForHash(std::string token);
		std::string & operator[](int n);
		size_t find(const std::string & tok); //Finds the position of the tok in the given token
		void push_back(const std::string & tok);// Adds it into the vector
		bool remove(const std::string & tok); //Removes the tok from the given token
		bool remove(size_t i); //Removes at position i from token
		void toLower(); //converts the text field to lower
		void trimFront(); //trim the newline character from the front
		bool merge(const std::string& firstTok, const std::string& secondTok);
		void clear();
		std::string show(bool showNewLines = false);
	private:
		std::vector<Token> _tokens;
		Toker* _pToker;
	};
}
#endif
