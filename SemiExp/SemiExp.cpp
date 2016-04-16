///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.5                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Darshan Masti Prakash, Syracuse University,          //
//				dmastipr@syr.edu									 //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
///////////////////////////////////////////////////////////////////////
/*
*  Public Interface
*  ----------------
*  	toker.attach(&in);
*	SemiExp semi(&toker);

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
* - added new functionalities to extract the semiexpression and analyze the tokens
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
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <algorithm>
#include "SemiExp.h" 
#include "../Tokenizer/Tokenizer.h" 

using namespace Scanner;
SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}
//This method is used to get the tokens and analyze it for semi expressions 
bool SemiExp::get(bool clear)
{
	bool semiStat = false;
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	_tokens.clear();
	std::string tempSemiExp = "";
	while (true)
	{
		std::string token = _pToker->getTok();
		if (token.empty())
			return false;
		if (token == "\n")
			continue;
		if (token == ";")
		{
			push_back(tempSemiExp + token);
			tempSemiExp = "";
			return true;
		}
		if (token == "{")
		{
			tempSemiExp += token;
			push_back(tempSemiExp);
			return true;
		}
		semiStat = getSemiExpHelper(token, tempSemiExp);
		if (semiStat == true)
			return true;
		else
			tempSemiExp = tempSemiExp + " " + token;
	}
	return false;
}
//helper get method to analyze semi expressions
bool SemiExp::getSemiExpHelper(std::string token, std::string &tempSemiExp)
{
	if (token == "for")
	{
		std::string nextTok = token;
		std::string prevTok = "";
		do
		{
			tempSemiExp = tempSemiExp + " " + nextTok;
			prevTok = nextTok;
			nextTok = _pToker->getTok();
		} while (nextTok != "{" && (!((nextTok != "{") && (prevTok == ")"))));
		if (nextTok != "{" && prevTok == ")")
			push_back(tempSemiExp);
		else if (nextTok == "{")
		{
			push_back(tempSemiExp + "{");
			_pToker->getTok();
		}
		tempSemiExp = ""; return true;
	}
	if (token == "}")
	{
		push_back(tempSemiExp + "}");
		return true;
	}
	if ((token == "public" || token == "protected" || token == "private"))
	{
		std::string tempNextExp = _pToker->getTok();
		if (tempNextExp == ":")
		{
			push_back(tempSemiExp + token + tempNextExp);
			tempSemiExp = "";
		}
		return true;
	}
	return getHelperForHash(token);

	return false;
}

bool SemiExp::getHelperForHash(std::string token)
{
	if (token == "#")
	{
		std::string tempNextExp = ""; std::string tempTok = token;
		tempNextExp = tempTok;
		do
		{
			std::string temp = _pToker->getTok();
			tempTok = temp;
			if (temp == "")
				break;
			if (temp.find(".h") != std::string::npos)
			{
				tempNextExp = tempNextExp + " " + temp;
				break;
			}
			else
				tempNextExp = tempNextExp + " " + temp;
		} while (tempTok != "\n");
		push_back(tempNextExp);
		return true;
	}
	return false;
}

// this method is used to get the item at index n
std::string& SemiExp::operator[](int n)
{
	size_t t = n;
	if (t < 0 || t >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}
//get the length of the token
size_t SemiExp::length()
{
	return _tokens.size();
}
//display the semi expression items
std::string SemiExp::show(bool showNewLines)
{
	std::cout << "\n  ";
	std::cout << "--";
	std::string sendSemiExp = "";
	for (auto token : _tokens)
		if (token != "\n" || showNewLines == true)
		{
			std::cout << token << " ";
			sendSemiExp = sendSemiExp + "," + token;
		}
	std::cout << "\n";
	return sendSemiExp;
}

//find the sub token from the main token collection
size_t SemiExp::find(const std::string & tok)
{
	for (size_t i = 0; i < _tokens[0].size(); i++)
	{
		if (_tokens[0].at(i) == tok[0])
		{
			bool statFound = false;
			for (size_t j = 0; j < tok.size(); j++)
			{
				if (_tokens[0].at(i + j) == tok[j])
					statFound = true;
				else
				{
					statFound = false;
					break;
				}
			}
			if (statFound == true)
				return i;
		}
	}
	return size_t(-1);
}

// this is used to push back the token into the token collection
void SemiExp::push_back(const std::string & tok)
{
	_tokens.push_back(tok);
}

// this is used to remove the sub token from the main token collection
bool SemiExp::remove(const std::string & tok)
{
	int pos = find(tok);
	_tokens[0].erase(pos, tok.size());
	return true;
}
//this is used to remove at position i in the token collection
bool SemiExp::remove(size_t i)
{
	_tokens[0].erase(i, 1);
	return true;
}
//convert the token characters tolower case
void SemiExp::toLower()
{
	for (size_t i = 0; i < _tokens.size(); i++)
		std::transform(_tokens[i].begin(), _tokens[i].end(), _tokens[i].begin(), ::tolower);
}
//trim front the new spaces in the data
void SemiExp::trimFront()
{
	for (size_t i = 0; i < _tokens.size(); i++)
	{
		if (_tokens[i] != "\n")
			break;
		else
			remove(i);
	}
}
bool SemiExp::merge(const std::string& firstTok, const std::string& secondTok)
{
	return true;
}
void SemiExp::clear()
{
	_tokens.clear();
}

#ifdef SEMI_EXPRESSION
//--------Test Stub for SemiExp ---------------
int main()
{
	Toker toker;
	std::string fileSpec = "../SemiExp/testSemiExp.txt";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";return 1;
	}
	toker.attach(&in);
	SemiExp semi(&toker);
	int countLower = 0;int countFind = 0;
	toker.returnComments(false);
	while (semi.get())
	{
		if (semi.length() > 0)
			semi.trimFront();
		if (countLower < 1 && semi.length() >0)
		{
			std::cout << "\n  -- semiExpression before trimming to lower --";
			semi.show();semi.toLower();
			std::cout << "\n  -- semiExpression after trimming to lower --";
			semi.show();
			std::cout << "\n  -- semiExpression before removing any character --";
			semi.show();semi.remove(1);
			std::cout << "\n  -- semiExpression after removing third character--";
			semi.show();countLower++;
		}
		else if (semi.length() > 0)
		{
			semi.toLower();
			std::cout << "\n";//  -- semiExpression --";
			semi.show();
			countLower++;
		}
		if (countFind < 1 && semi.length()>0)
		{
			if (semi.find("include") != size_t(-1))
			{
				std::cout << "\n  -- semiExpression before removing token --include-- from -- #include <string> --";
				semi.show();
				semi.remove("include");
				countFind++;
				std::cout << "\n  -- semiExpression after removing token --include-- from -- #include <string> --";
				semi.show();
			}
		}
	}
	return 0;
}
#endif // SEMI_EXPRESSION
