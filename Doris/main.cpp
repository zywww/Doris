#include <iostream>	
#include <cstdlib>
#include <string>
#include "Lexer.h"

using std::string;
using std::endl;
using std::cout;
using std::cin;

void testLexer()
{
	Lexer lex1(string("testt"));
	Lexer lex2(string("|*+?^${}[]()abc,\\\\ \\z\\n\\r\\t\\v\\w\\W\\s\\S\\d\\D\\b\\B\\a\\x30\\x40"));
	//Lexer lex3(string("\\"));
	//Lexer lex4(string("\\xq"));
	//Lexer lex5(string("\\xqq"));
	//Lexer lex6(string("\\x"));
	Lexer lex7(string("\\1abc"));
	Lexer lex8(string("\\0abc"));
	auto temp = lex7.GetNextToken();
	while (temp.type_ != TokenType::END)
		temp = lex7.GetNextToken();

	cout << "ÏÂ¶Ïµã" << endl;
}

void test()
{
	testLexer();
}

int main()
{
	test();

	system("pause");
	return 0;
}