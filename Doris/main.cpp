#include <iostream>	
#include <cstdlib>
#include <string>
#include "Lexer.h"
#include "Parser.h"

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
	Lexer lex28(string("-\\-"));
	Lexer lex29(string(""));
	Lexer lex30("\\w\\W\\s\\S\\d\\D");

	cout << "下断点" << endl;
}

void testParser()
{
	Parser ps1(string(""));
	Parser ps9(string("a||a"));
	Parser ps2(string("()"));
	Parser ps3(string("a|b"));
	Parser ps4(string("ab|cddd|efg"));
	Parser ps5(string("a* "));
	Parser ps6(string("a*b+c?"));
	Parser ps7(string("d{3}"));
	Parser ps8(string("e{4,}f{5,6}"));
	Parser ps10("a*?b+?c??d{3}?e{4,}?f{5,6}?");
	Parser ps11("[abc]");
	Parser ps12("[^abc]");
	Parser ps13("[abc]{3,5}");
	//Parser ps14("^*");
	//Parser ps15("e{3,2}");
	//Parser ps16("a{2147483647,2147483648}");
	Parser ps15(string("^$\\b\\B	"));
	//Parser ps16(string("\\3+"));
	Parser ps17("(?<name>regex)*(\\k<name>*)");
	//Parser ps18("[ca-b^de-f]*");
	Parser ps19("[ca-bde-f]*");
	Parser ps20("(re)");
	//Parser ps21("(reg(ex)");
	Parser ps22("(reg(ex))");
	Parser ps23("(?:regex)");
	Parser ps24("(?<name>regex)");
	Parser ps25("(?=regex)");
	Parser ps26("(?!regex)");
	Parser ps27("()\\1");
	//Parser ps28("a*b**");
	Parser ps29("a*|b+|(?:((re|ad{2,3}))*?)|(5)|");
	Parser ps30("|a");
	Parser ps31("(asd|asd)");
	Parser ps32("((a))");
	Parser ps33("|||");
	Parser ps34("(||)asd");
	//Parser ps35("[||]");
	Parser ps36(".\\w\\W\\s\\S\\d\\D");
	Parser ps38("\\w\\W\\s\\S\\d\\D*");
	Parser ps37(".");
	Parser ps39("[]");

	cout << "下断点" << endl;
}

void test()
{
	//testLexer();
	testParser();
}

int main()
{
	test();
	

	system("pause");
	return 0;
}