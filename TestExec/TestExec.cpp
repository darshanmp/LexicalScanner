/////////////////////////////////////////////////////////////////////
// TestExec.cpp - test layer					                   //
// ver 1.0                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Darshan Masti Prakash, Syracuse University,        //
//				dmastipr@syr.edu								   //
/////////////////////////////////////////////////////////////////////
/*
Test layer to automate the testing of SemiExp and Tokenizer packages
*/
/*
*  Public Interface
*  ----------------
*  void TestR5();

* Package Operations:
* -------------------
* This package contains different functionalities to meet the requriements and automate the testing
* so that it can be run in future without any supervision
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp , SemiExp.cpp, SemiExp.h
* Build Command: devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 09 Feb 2016
* - first release of new design
*
*/
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"

using namespace Scanner;
class TestExec
{
public:
	TestExec();
	~TestExec();
	void DisplayInputStream(std::string);
	void Req1();
	void Req2();
	void Req3();
	void Req4();
	void Req5();
	void Req6();
	void Req7AndReq8A();
	void Req7AndReq8B(SemiExp &semi, int &countLower, int &countFind);
	void Req9A();
	void Req9B(SemiExp &semi, int &countLower, int &countFind);
	void Req10();
private:
};
TestExec::TestExec()
{
}
TestExec::~TestExec()
{
}

//  Requirement 7 and 8 is displayed here 
void TestExec::Req7AndReq8B(SemiExp &semi, int &countLower, int &countFind)
{
	while (semi.get())
	{
		if (semi.length() > 0)
			semi.trimFront();
		if (countLower < 1 && semi.length() >0)
		{
			std::cout << "\n  -- semiExpression before trimming to lower --";
			semi.show();
			semi.toLower();
			std::cout << "\n  -- semiExpression after trimming to lower --";
			semi.show();
			std::cout << "\n  -- semiExpression before removing any character --";
			semi.show();
			semi.remove(1);
			std::cout << "\n  -- semiExpression after removing third character--";
			semi.show();
			countLower++;
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
}

//This is used to display input stream and display the input file contents
void TestExec::DisplayInputStream(std::string fileName)
{
	std::string line;
	std::cout << "----------Input data -------" << "\n\n";
	std::ifstream inputfile(fileName);
	if (inputfile.is_open()) {
		while (getline(inputfile, line)) {
			std::cout << line << "\n";
		}
		inputfile.close();
	}
	else
		std::cout << "Unable to open file";
	std::cout << "\n----------Input data -------" << "\n\n";
}
//Demonstrating Requirement #1
void TestExec::Req1()
{
	std::cout << "Demonstrating Requirement #1 : Visual Studio 2015 and Console based apps have been used.This can be verified by checking the Project folder settings" << "\n\n";
}
//Demonstrating Requirement #2
void TestExec::Req2()
{
	std::cout << "Demonstrating Requirement #2 : Tokenizer.cpp Line #399 and Line #380 for allocating/delocating memory new and delete using heap" << "\n\n";
}
//Demonstrating Requirement #3
void TestExec::Req3()
{
	std::cout << "Demonstrating Requirement #3 : Tokenizer.cpp , SemiExpression, TestExec and ITokCollection projects can be seen in the solution" << "\n\n";
}
//Demonstrating Requirement #4
void TestExec::Req4()
{
	std::cout << "Demonstrating Requirement #4" << "\n\n";
	std::string fileSpec = "Req4.txt";
	DisplayInputStream(fileSpec);
	std::cout << "The items being displayed here are : " << "\n";
	std::cout << "1: Alphanumeric tokens " << "\n";
	std::cout << "2: Punctuation tokens " << "\n";
	std::cout << "3: Special one and two character tokens " << "\n";
	std::cout << "4: Change special one and two character tokens " << "\n";
	std::cout << "5: Change special one and two character tokens " << "\n";
	std::cout << "6: C Style comments " << "\n";
	std::cout << "7: C++ Style comments " << "\n";
	std::cout << "8: Quotes strings comments " << "\n";
	std::cout << "9: Quotes strings comments Special cases" << "\n";
	static std::istream* _pIn;
	std::ifstream in(fileSpec);
	if (!in.good())
		std::cout << "\n  can't open " << fileSpec << "\n\n";
	Scanner::Toker toker;
	toker.attach(&in);
	std::cout << "The output tokens are :\n";
	do
	{
		toker.returnComments(true); //setting the comments to true so that c & c++ comments are returned as tokens
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in.good());
	std::cout << "\n\nTesting change of special characters";
	std::string singleChars = ":,.,+,";
	std::string doubleChars = "+=";
	Scanner::Toker::reset();
	Scanner::Toker toker1;
	toker1.returnComments(true);
	toker1.setSpecialSingleCharsTok(singleChars); //set new special single characters
	toker1.setSpecialCharPairsTok(doubleChars);//set new special double characters
	in.clear();
	toker1.attach(&in);
	std::cout << "\n  new special single tokens: " << singleChars;
	std::cout << "\n  new special double tokens: " << doubleChars;
}
//Demonstrating Requirement #5
void TestExec::Req5()
{
	std::cout << "\nDemonstrating Requirement #5 : Line #432 & # 487 in Tokenizer.cpp" << "\n\n";
}
//Demonstrating Requirement #6
void TestExec::Req6()
{
	std::cout << "\nDemonstrating Requirement #6 : Line #64 in SemiExp.cpp" << "\n\n";
}
//Demonstrating Requirement #7 and #8
void TestExec::Req7AndReq8A()
{
	std::cout << "\n\n Demonstrating Requirement #7 and Requirement #8" << "\n\n";
	std::string fileSpec = "Req78.txt"; //Read from Req78.txt file
	DisplayInputStream(fileSpec);
	std::cout << "The semi-expressions being displayed here are : " << "\n";
	std::cout << "1: Semicolon " << "\n";
	std::cout << "2: Open Brace " << "\n";
	std::cout << "3: Closed Brace " << "\n";
	std::cout << "4: # " << "\n";
	std::cout << "5: Colon(:) followed by Public/Private/Protected" << "\n";
	std::cout << "6: Special expressions like (for(;;) - Demonstrating Requirement #8 " << "\n";
	Scanner::Toker::reset();
	Scanner::Toker toker;
	static std::istream* _pIn;
	std::ifstream in(fileSpec);
	if (!in.good())
		std::cout << "\n  can't open " << fileSpec << "\n\n";
	toker.attach(&in);
	SemiExp semi(&toker);
	int countLower = 0;
	int countFind = 0;
	toker.returnComments(false);
	Req7AndReq8B(semi, countLower, countFind);
}

//Demonstrating Requirement #9 
void TestExec::Req9A()
{
	std::cout << "\n\n\n\n Demonstrating Requirement #9" << "\n\n";
	std::string fileSpec = "Req9.txt";
	DisplayInputStream(fileSpec);
	std::cout << "The semi-expressions being displayed here are : " << "\n";
	std::cout << "1: Get : Please refer Line #21 in SemiExp.cpp" << "\n";
	std::cout << "2: Find : Please refer Line #139 in SemiExp.cpp" << "\n";
	std::cout << "3: Merge : Please refer Line #192 in SemiExp.cpp " << "\n";
	std::cout << "3: Operator : Please refer Line #102 in SemiExp.cpp " << "\n";
	std::cout << "4: Push_Back: Please refer Line #162 in SemiExp.cpp " << "\n";
	std::cout << "5: Remove: Please refer Line #166 & #172 in SemiExp.cpp " << "\n";
	std::cout << "6: toLower :Will be demonstrated now" << "\n";
	std::cout << "7: trimFront :Will be demonstrated now" << "\n";
	std::cout << "8: Clear:Will be demonstrated now" << "\n";
	std::cout << "9: Show :Will be demonstrated now" << "\n";
	Scanner::Toker::reset();
	Scanner::Toker toker;
	static std::istream* _pIn;
	std::ifstream in(fileSpec);
	if (!in.good())
		std::cout << "\n  can't open " << fileSpec << "\n\n";
	toker.attach(&in);
	SemiExp semi(&toker);
	int countLower = 0;
	int countFind = 0;
	toker.returnComments(false);
	Req9B(semi, countLower, countFind);
}
//Demonstrating Requirement #9
void TestExec::Req9B(SemiExp &semi, int &countLower, int &countFind)
{
	while (semi.get())
	{
		if (semi.length() > 0)
		{
			std::cout << "Trimmed from the front\n";
			semi.trimFront();
		}
		if (countLower < 1 && semi.length() >0)
		{
			std::cout << "\n  -- semiExpression before trimming to lower --";
			semi.show();
			semi.toLower();
			std::cout << "\n  -- semiExpression after trimming to lower --";
			semi.show();
			std::cout << "\n  -- semiExpression before removing any character --";
			semi.show();
			semi.remove(1);
			std::cout << "\n  -- semiExpression after removing third character--";
			semi.show();
			countLower++;
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
}
//Demonstrating Requirement #10
void TestExec::Req10()
{
	std::cout << "Demonstrating Requirement #610 :Please refer TestExec.cpp file to see the automated test cases that would be run on the 2 packages\n";
}

int main()
{
	//main entry for the program
	TestExec test;
	test.Req1();
	test.Req2();
	test.Req3();
	test.Req4();
	test.Req5();
	test.Req6();
	test.Req7AndReq8A();
	test.Req9A();
	test.Req10();
	std::getchar();
	return 0;
}


