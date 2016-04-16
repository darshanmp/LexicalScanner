//////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                    //
// ver 3.5                                                          //
// Language:    C++, Visual Studio 2015                             //
// Application: Parser component, CSE687 - Object Oriented Design   //
// Author:      Darshan Masti Prakash, Syracuse University,         //
//				dmastipr@syr.edu								    //
// Source:      Jim Fawcett, Syracuse University, CST 4-187         //
//////////////////////////////////////////////////////////////////////
/*
Tokenizer.cpp splits the data into different tokens based on some rules and delimiters
*/
/*
*  Public Interface
*  ----------------
*  	Toker toker;
*	toker.attach(&in);
*	std::string tok = toker.getTok();
*
* Package Operations:
* -------------------
* This package provides a public Toker class and private ConsumeState class.
* Toker reads words from a std::stream, throws away whitespace and comments
* and returns words from the stream in the order encountered.  Quoted
* strings and certain punctuators and newlines are returned as single tokens.
*
* This is a new version, based on the State Design Pattern.  Older versions
* exist, based on an informal state machine design.
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 3.5 : 09 Feb 2016
* - added new method declarations in the tokenizer.cpp file for splitting it into different tokens
* ver 3.4 : 03 Feb 2016
* - fixed bug that prevented reading of last character in source by
*   clearing stream errors at beginning of ConsumeState::nextState()
* ver 3.3 : 02 Feb 2016
* - declared ConsumeState copy constructor and assignment operator = delete
* ver 3.2 : 28 Jan 2016
* - fixed bug in ConsumeState::nextState() by returning a valid state
*   pointer if all tests fail due to reaching end of file instead of
*   throwing logic_error exception.
* ver 3.1 : 27 Jan 2016
* - fixed bug in EatCComment::eatChars()
* - removed redundant statements assigning _pState in derived eatChars()
*   functions
* - removed calls to nextState() in each derived eatChars() and fixed
*   call to nextState() in ConsumeState::consumeChars()
* ver 3.0 : 11 Jun 2014
* - first release of new design
*

*/
#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>

namespace Scanner
{
	//ConsumeState class which is the main class from which Tokenizer is derived from
	class ConsumeState
	{
	public:
		ConsumeState();
		ConsumeState(const ConsumeState&) = delete;
		ConsumeState& operator=(const ConsumeState&) = delete;
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		ConsumeState* nextState(); //set the next state 
		ConsumeState* nextState1(int chNext);
		bool setSpecialSingleChars(const std::string&);//set special single chars
		bool setSpecialCharPairs(const std::string&); //set special double chars
		bool doReturnComments(bool statComments); //to decide whether to return comments 
		std::vector<std::string> split(const std::string& src);
		static void reset();
	protected:
		static std::string token;
		static std::istream* _pIn;
		static int prevChar;
		static int currChar;
		static bool first_var;
		static bool first_dvar;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatNewline;
		static ConsumeState* _pEatQuotedString;
		static ConsumeState* _pEatSpecialDoubleChar;
		static ConsumeState* _pEatSpecialSingleChar;
		std::vector<std::string> specialSingleChars{ "\n", "<", ">", "{", "}", "[", "]", "(", ")", ":", "=", "+", "-", "*", ".",";" };
		std::vector<std::string> specialDoubleChars{ "<<", ">>", "::", "++", "--", "==", "+=", "-=", "*=", "/=" };
		static bool statComments;
	};
}

using namespace Scanner;

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
int ConsumeState::prevChar;
int ConsumeState::currChar;
bool ConsumeState::first_var = true;
bool ConsumeState::first_dvar = true;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
ConsumeState* ConsumeState::_pEatQuotedString = nullptr;
ConsumeState* ConsumeState::_pEatSpecialDoubleChar = nullptr;
ConsumeState* ConsumeState::_pEatSpecialSingleChar = nullptr;
bool ConsumeState::statComments = false;

void testLog(const std::string& msg);
//----< called by Toker constructor >--------------------------------
void ConsumeState::reset()
{
	first_var = first_dvar = true;
}
ConsumeState* ConsumeState::nextState1(int chNext)
{
	try
	{
		std::string currStr;
		currStr.push_back(currChar);
		std::string stNext;
		stNext.push_back(chNext);
		if (std::find(specialDoubleChars.begin(), specialDoubleChars.end(), currStr + stNext) != specialDoubleChars.end())
		{
			testLog("state: eatSpecialDoubleChar");
			return _pEatSpecialDoubleChar;
		}
		if (std::find(specialSingleChars.begin(), specialSingleChars.end(), currStr) != specialSingleChars.end())
		{
			testLog("state: eatSpecialSingleChar");
			return _pEatSpecialSingleChar;
		}
		if (ispunct(currChar))
		{
			testLog("state: eatPunctuator");
			return _pEatPunctuator;
		}
		if (!_pIn->good())
			return _pEatWhitespace;
	}
	catch (std::logic_error& ex)
	{
		std::cout << "\n  " << ex.what();
	}
}

std::vector<std::string> ConsumeState::split(const std::string& data)
{
	std::vector<std::string> tokens;
	try
	{
		std::stringstream ss(data);
		std::string acc;
		while (getline(ss, acc, ',')) 
		{
			tokens.push_back(acc);
		}
	}
	catch (std::logic_error& ex)
	{
		std::cout << "\n  " << ex.what();
	}
	return tokens;
}
//----< replace one char tokens >----------------------------

bool ConsumeState::setSpecialSingleChars(const std::string& commaTok)
{
	std::vector<std::string> result = split(commaTok);
	specialSingleChars.clear();
	for (std::string item : result)
		specialSingleChars.push_back(item);
	return true;
}
//----< replace two char tokens >----------------------------
bool ConsumeState::setSpecialCharPairs(const std::string& commaTok)
{
	try
	{
		std::vector<std::string> result = split(commaTok);
		specialDoubleChars.clear();
		for (std::string item : result)
			specialDoubleChars.push_back(item);
	}
	catch (std::logic_error& ex)
	{
		std::cout << "\n  " << ex.what();
	}
	return true;
}
//----< turn on\off returning comments as tokens >-------------------
bool ConsumeState::doReturnComments(bool statCom)
{
	statComments = statCom;
	return true;
}
//----< decide which state to use next >-----------------------------
ConsumeState* ConsumeState::nextState()
{
	if (!(_pIn->good()))
		return nullptr;
	int chNext = _pIn->peek();
	if (chNext == EOF)
		_pIn->clear();
	if (std::isspace(currChar) && currChar != '\n')
	{
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}
	if (currChar == '/' && chNext == '/')
	{
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}
	if (currChar == '/' && chNext == '*')
	{
		testLog("state: eatCComment");
		return _pEatCComment;
	}
	if (currChar == '\n')
	{
		testLog("state: eatNewLine");
		return _pEatNewline;
	}
	if (std::isalnum(currChar))
	{
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}
	if (currChar == '"')
	{
		testLog("state: eatQuotedString");
		return _pEatQuotedString;
	}
	return nextState1(chNext);

}
//----< class that consumes whitespace >-----------------------------
class EatWhitespace : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating whitespace";
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (std::isspace(currChar) && currChar != '\n');
	}
};
//----< class that consumes C++ comment >----------------------------
class EatCppComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			token += currChar;
			currChar = _pIn->get();
		} while (currChar != '\n');
		if (statComments == false)
			token.clear();
	}
};

//----< class that consumes C comment >------------------------------
class EatCComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			token += currChar;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		token += currChar;
		token += _pIn->get();
		currChar = _pIn->get();
		if (statComments == false)
			token.clear();
	}
};

//----< class that eats punctuators >--------------------------------
class EatPunctuator : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating punctuator";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (ispunct(currChar));
	}
};

//----< class that eats alphanumeric characters >--------------------
/*
* treats underscore '_' as alphanumeric character
*/
class EatAlphanum : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar) || currChar == '_');
	}
};

//----< class that eats one and two character tokens >---------------
class EatSpecialDoubleCharacters : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating special double chars";
		if (!_pIn->good())  // end of stream
			return;
		token += currChar;
		token += _pIn->peek();
		currChar = _pIn->get();
		currChar = _pIn->get();
	}
};
//----< class that eats one and two character tokens >---------------
class EatSpecialSingleCharacters : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating special single chars";
		if (!_pIn->good())  // end of stream
			return;
		token += currChar;
		currChar = _pIn->get();
	}
};
//----< class that eats newline >-----------------------
class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};
//----< class that eats quoted string >-----------------------
class EatQuotedString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		int escapeCharOn = 0; //initial : 0 true : 1 false :2
		//std::cout << "\n  eating Quoted String";
		do {
			if (escapeCharOn == 1)
			{
				token += currChar;
				escapeCharOn = 2;
				currChar = _pIn->get();
				if (!_pIn->good())  // end of stream
					return;
			}
			else
			{
				if (!_pIn->good())  // end of stream
					return;
				if (escapeCharOn != 0)
					token += currChar;
				escapeCharOn = 2;
				currChar = _pIn->get();
				std::string combChar;
				combChar += currChar;
				combChar += _pIn->peek();
				if (combChar == "\\\"" || combChar == "\\\'")
				{
					currChar = _pIn->get();
					escapeCharOn = 1;
				}
			}
		} while (currChar != '"' || escapeCharOn == 1);
		_pIn->get();
		currChar = _pIn->get();
	}
};
//----< constructor creates derived state instances only once >------
ConsumeState::ConsumeState()
{
	if (first_var)
	{
		first_var = false;
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatNewline = new EatNewline();
		_pEatQuotedString = new EatQuotedString();
		_pEatSpecialDoubleChar = new EatSpecialDoubleCharacters();
		_pEatSpecialSingleChar = new EatSpecialSingleCharacters();
		_pState = _pEatWhitespace;
	}
}

//----< destructor deletes derived state instances only once >-------
ConsumeState::~ConsumeState()
{
	if (first_dvar)
	{
		first_dvar = false;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatQuotedString;
		delete _pEatNewline;
		delete _pEatSpecialDoubleChar;
		delete _pEatSpecialSingleChar;
	}
}
//----< construct toker starting in state EatWhitespace >------------
Toker::Toker() : pConsumer(new EatWhitespace()) {}
void Toker::reset()
{
	ConsumeState::reset();
}
Toker::~Toker() { delete pConsumer; }

bool Toker::returnComments(bool doReturnComments)
{
	return pConsumer->doReturnComments(doReturnComments);
}

bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}
//----< collect token generated by ConsumeState >--------------------
std::string Toker::getTok()
{
	while (true)
	{
		if (!pConsumer->canRead())
			return "";
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;
	}
	pConsumer->nextState();
	return pConsumer->getTok();
}


bool Toker::canRead() { return pConsumer->canRead(); }
//----< set one and two char tokens >--------------------------------
bool Toker::setSpecialSingleCharsTok(const std::string& commaTokens)
{
	return pConsumer->setSpecialSingleChars(commaTokens);
}
//----< set one and two char tokens >--------------------------------
bool Toker::setSpecialCharPairsTok(const std::string& commaTokens)
{
	return pConsumer->setSpecialCharPairs(commaTokens);
}
void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}


//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	try
	{
		//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
		//std::string fileSpec = "../Tokenizer/Tokenizer.h";
		std::string fileSpec = "testTokenizer.txt";

		std::ifstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  can't open " << fileSpec << "\n\n";
			return 1;
		}
		Toker toker;

		toker.attach(&in);
		do
		{
			std::string tok = toker.getTok();
			if (tok == "\n")
				tok = "newline";
			std::cout << "\n -- " << tok;
		} while (in.good());

		std::cout << "\n\n";
		std::getchar(); //Remove
	}
	catch (std::logic_error& ex)
	{
		std::cout << "\n  " << ex.what();
	}
	return 0;
}
#endif
