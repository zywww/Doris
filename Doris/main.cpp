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
	//Parser ps39("[]");
	Parser ps40("[1-\\w]");
	//Parser ps41("[-\\b]");
	//Parser ps42("\\");
	Parser ps43("[^^|*+?^${}()<>.-\\w\\W\\d\\D\\s\\S]");
	//Parser ps44("[\\k]");
	//Parser ps45("[^]");
	Parser ps46("()*");

	cout << "下断点" << endl;
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

	cout << "下断点" << endl;
}

void testMatch()
{
	
/*1 0*/	cout << "re1  " << Regex("ab").Match("abc") << endl;
/*2 1*/	cout << "re2  " << Regex("a*").Match("aaaa") << endl;
/*3 1*/	cout << "re3  " << Regex("a*").Match("a") << endl;
/*4 1*/	cout << "re4  " << Regex("a*").Match("") << endl;
/*5 0*/	cout << "re5  " << Regex("a*").Match("aaaaaaaaaaaab") << endl;
/*6 1*/	cout << "re6  " << Regex("ab*").Match("abb") << endl;
/*7 1*/	cout << "re7  " << Regex("ab*").Match("abbb") << endl;
/*8 1*/	cout << "re8  " << Regex("ab*").Match("a") << endl;
/*9 0*/	cout << "re9  " << Regex("b?").Match("bb") << endl;
/*10 0*/cout << "re10 " << Regex("b{0}").Match("bb") << endl;
/*11 0*/cout << "re11 " << Regex("a|bbb").Match("bb") << endl;
/*12 0*/cout << "re12 " << Regex("a*|bbb").Match("bb") << endl;
/*13 1*/cout << "re13 " << Regex("a|b*").Match("bb") << endl;	
/*14 1*/cout << "re14 " << Regex("(a*)*").Match("") << endl;
/*15 1*/cout << "re15 " << Regex("(a*)*").Match("aaaa") << endl;
/*16 0*/cout << "re16 " << Regex("(a*)*|bbb").Match("bbb") << endl;
/*17 1*/cout << "re17 " << Regex("[ab]*").Match("bbb") << endl;
/*18 1*/cout << "re18 " << Regex("([ab]*)").Match("bbb") << endl;
/*19 0*/cout << "re19 " << Regex("a+").Match("") << endl;
/*20 1*/cout << "re20 " << Regex("a+").Match("a") << endl;
/*22 1*/cout << "re22 " << Regex("a+").Match("aaa") << endl;
/*23 0*/cout << "re23 " << Regex("a?").Match("aaa") << endl;
/*24 1*/cout << "re24 " << Regex("a?").Match("a") << endl;
/*25 1*/cout << "re25 " << Regex("a?").Match("") << endl;
/*26 0*/cout << "re26 " << Regex("a{2}").Match("aaa") << endl;
/*27 1*/cout << "re27 " << Regex("a{2}").Match("aa") << endl;
/*28 1*/cout << "re28 " << Regex("a{2,}").Match("aaaaa") << endl;
/*29 1*/cout << "re29 " << Regex("a{2,}").Match("aa") << endl;
/*30 0*/cout << "re30 " << Regex("a{2,}").Match("a") << endl;
/*31 0*/cout << "re31 " << Regex("a{2,3}").Match("a") << endl;
/*32 1*/cout << "re32 " << Regex("a{2,3}").Match("aa") << endl;
/*33 1*/cout << "re33 " << Regex("a{2,3}").Match("aaa") << endl;
/*34 0*/cout << "re34 " << Regex("a{2,3}").Match("aaaa") << endl;
/*35 0*/cout << "re35 " << Regex("[a-ce]*").Match("abcd") << endl;
/*36 1*/cout << "re36 " << Regex("[a-ce]*").Match("abc") << endl;
/*37 1*/cout << "re37 " << Regex("[a-ce]*").Match("abce") << endl;
/*38 1*/cout << "re38 " << Regex("[a-ce-r]*").Match("abce") << endl;
/*39 0*/cout << "re39 " << Regex("[a-ce-r]*").Match("abces") << endl;
/*40 1*/cout << "re40 " << Regex("[a-c\\w]*").Match("abasdlkjaslkdz13152_ce") << endl;
/*41 0*/cout << "re41 " << Regex("[\\s]*").Match("abasdlkjaslkdz13152_ce") << endl;
/*42 1*/cout << "re42 " << Regex("[\\s]*").Match("   \f\n\r\t\v") << endl;
/*43 1*/cout << "re43 " << Regex("(abc)\\1").Match("abcabc") << endl;
Regex re("(abc)\\1");
cout << re.Match("abcabc") << endl;




	cout << "下断点" << endl;
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