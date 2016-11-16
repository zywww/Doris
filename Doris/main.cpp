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
	// 这种测试方法，下次看就忘记测试用例的目的
	// 不能做回归测试
	Lexer lex1(string("testt"));
	Lexer lex2(string("|*+?^${}[]()abc,\\\\ \\z\\n\\r\\t\\v\\w\\W\\s\\S\\d\\D\\b\\B\\a\\x30\\x40"));
	//Lexer lex3(string("\\"));
	//Lexer lex4(string("\\xq"));
	//Lexer lex5(string("\\xqq"));
	//Lexer lex6(string("\\x"));
	Lexer lex7(string("\\1abc"));
	Lexer lex8(string("\\0abc"));
	Lexer lex9(string(R"(\1)"));
	Lexer lex10(string("\\k"));
	auto temp = lex7.GetNextToken();
	while (temp.type_ != TokenType::END)
		temp = lex7.GetNextToken();

	Lexer lex11(string("aasd\\a\\1abc"));
	Lexer lex12(string("asd\\kasd"));
	Lexer lex13(string("$asd"));
	Lexer lex14(string("asd\\b"));
	Lexer lex15(string("\\B"));
	Lexer lex16(string("^"));
	Lexer lex17(string("a[^asd]"));
	Lexer lex18(string("^[^a]"));
	Lexer lex19(string("a{2,}?"));
	Lexer lex20(string("a*?"));
	Lexer lex21(string("a+?"));
	Lexer lex22(string("a??"));
	Lexer lex23(string("a(asd)"));
	Lexer lex24(string("a(?:asd)"));
	Lexer lex25(string("a(?<name>w+)"));
	Lexer lex26(string("a(?=ad)"));
	Lexer lex27(string("a(?!ad)"));

	cout << "下断点" << endl;
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