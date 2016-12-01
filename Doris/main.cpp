#include <iostream>	
#include <cstdlib>
#include <string>
#include "Lexer.h"
#include "Parser.h"
#include "Regex.h"

using std::string;
using std::endl;
using std::cout;
using std::cin;

void testLexer()
{
	// ���ֲ��Է������´ο������ǲ���������Ŀ��
	// �������ع����
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

	cout << "�¶ϵ�" << endl;
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
	//Parser ps39("[]");
	Parser ps40("[1-\\w]");
	//Parser ps41("[-\\b]");
	//Parser ps42("\\");
	Parser ps43("[^^|*+?^${}()<>.-\\w\\W\\d\\D\\s\\S]");
	//Parser ps44("[\\k]");
	//Parser ps45("[^]");
	Parser ps46("()*");

	cout << "�¶ϵ�" << endl;
}

void testNFA()
{
	Regex re1("abc");
	Regex re2("ab|cd");
	Regex re3("");
	Regex re4("()");
	Regex re5("|");
	Regex re6("(|)");
	Regex re7("b*");
	Regex re8("b*?");
	Regex re9("b+");
	Regex re10("b+?");
	Regex re11("b?");
	Regex re12("b??");
	Regex re13("b{2,4}");
	Regex re14("b{2,4}?");
	Regex re15("b{2,}");
	Regex re16("b{2,}?");
	Regex re17("b{2}");
	Regex re18("b{2}?");
	Regex re19("b{0,3}");
	Regex re20("b{0,3}?");
	Regex re21("[1-2abc\\w]");
	Regex re22("(a)\\1");
	Regex re23("(?<name>ab)\\k<name>");
	Regex re24("\\b\\B$^");

	cout << "�¶ϵ�" << endl;
}

void testMatch()
{
	cout << "re1  " << Regex("ab").Match("abc") << endl;
	cout << "re2  " << Regex("a*").Match("aaaa") << endl;
	cout << "re3  " << Regex("a*").Match("a") << endl;
	cout << "re4  " << Regex("a*").Match("") << endl;
	cout << "re5  " << Regex("a*").Match("aaaaaaaaaaaab") << endl;
	cout << "re6  " << Regex("ab*").Match("abb") << endl;
	cout << "re7  " << Regex("ab*").Match("abbb") << endl;
	cout << "re8  " << Regex("ab*").Match("a") << endl;
	cout << "re9  " << Regex("b?").Match("bb") << endl;
	cout << "re10 " << Regex("b{0}").Match("bb") << endl;
	Regex re("b{0}");
	cout << "�¶ϵ�" << endl;
}

void test()
{
	//testLexer();
	//testParser();
	//testNFA();
	testMatch();
}

int main()
{
	test();
	

	system("pause");
	return 0;
}