#include <iostream>	
#include <cstdlib>
#include <string>
#include <cassert>
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
	assert(Regex("ab").Match("abc") == false);
	assert(Regex("a*").Match("aaaa") == true);
	assert(Regex("a*").Match("a") == true);
	assert(Regex("a*").Match("") == true);
	assert(Regex("a*").Match("aaaaaaaaaaaab") == false);
	assert(Regex("ab*").Match("abb") == true);
	assert(Regex("ab*").Match("abbb") == true);
	assert(Regex("ab*").Match("a") == true);
	assert(Regex("b?").Match("bb") == false);
	assert(Regex("b{0}").Match("bb") == false);
	assert(Regex("a|bbb").Match("bb") == false);
	assert(Regex("a*|bbb").Match("bb") == false);
	assert(Regex("a|b*").Match("bb") == true);
	assert(Regex("(a*)*").Match("") == true);
	assert(Regex("(a*)*").Match("aaaa") == true);
	assert(Regex("(a*)*|bbb").Match("bbb") == false);
	assert(Regex("[ab]*").Match("bbb") == true);
	assert(Regex("([ab]*)").Match("bbb") == true);
	assert(Regex("a+").Match("") == false);
	assert(Regex("a+").Match("a") == true);
	assert(Regex("a+").Match("aaa") == true);
	assert(Regex("a?").Match("aaa") == false);
	assert(Regex("a?").Match("a") == true);
	assert(Regex("a?").Match("") == true);
	assert(Regex("a{2}").Match("aaa") == false);
	assert(Regex("a{2}").Match("aa") == true);
	assert(Regex("a{2,}").Match("aaaaa") == true);
	assert(Regex("a{2,}").Match("aa") == true);
	assert(Regex("a{2,}").Match("a") == false);
	assert(Regex("a{2,3}").Match("a") == false);
	assert(Regex("a{2,3}").Match("aa") == true);
	assert(Regex("a{2,3}").Match("aaa") == true);
	assert(Regex("a{2,3}").Match("aaaa") == false);
	assert(Regex("[a-ce]*").Match("abcd") == false);
	assert(Regex("[a-ce]*").Match("abc") == true);
	assert(Regex("[a-ce]*").Match("abce") == true);
	assert(Regex("[a-ce-r]*").Match("abce") == true);
	assert(Regex("[a-ce-r]*").Match("abces") == false);
	assert(Regex("[a-c\\w]*").Match("abasdlkjaslkdz13152_ce") == true);
	assert(Regex("[\\s]*").Match("abasdlkjaslkdz13152_ce") == false);
	assert(Regex("[\\s]*").Match("   \f\n\r\t\v") == true);
	assert(Regex("(abc)\\1").Match("abcabc") == true);
	assert(Regex("(abc)\\1*").Match("abcabc") == true);
	assert(Regex("(abc)\\1*").Match("abcabcabcabc") == true);
	assert(Regex("(abc)\\1*").Match("abc") == true);
	assert(Regex("()*").Match("abcabc") == false);
	assert(Regex("(?<name>abc)").Match("abc") == true);
	assert(Regex("(?<name>abc)\\k<name>").Match("abc") == false);
	assert(Regex("(?<name>abc)\\k<name>*").Match("abc") == true);
	assert(Regex("(?<name>abc)\\k<name>*").Match("abcabcabc") == true);
	assert(Regex("(?<name>abc)\\k<name>*").Match("abcabca") == false);
	assert(Regex("\\ba").Match("a") == true);
	assert(Regex("\\ba\\b").Match("a") == true);
	assert(Regex("\\ba\\b").Match("ab") == false);
	assert(Regex("\\ba\\B").Match("a") == false);
	assert(Regex("\\ba\\Bb").Match("ab") == true);
	assert(Regex("^ab").Match("ab") == true);
	assert(Regex("a^b").Match("ab") == false);
	assert(Regex("a$").Match("a") == true);
	assert(Regex("a^\\nb").Match("a\nb") == true);
	assert(Regex("((\\w) \\2)*").Match("a ab b") == true);
	assert(Regex("(a*?)").Match("aa") == true);
	assert(Regex("(?=ab)ab").Match("ab") == true);
	assert(Regex("(?!ab)ab").Match("ab") == false);
	assert(Regex("(?=a*)b").Match("b") == true);
	assert(Regex("(?=a*)b").Match("bc") == false);
	assert(Regex("(?=a*)b").Match("aaaaaaab") == false);
	assert(Regex("(?=$)").Match("") == true);
	assert(Regex("(?!$)").Match("") == false);
	assert(Regex("(?=(a))\\1").Match("a") == true);



	cout << "下断点" << endl;
}

void test()
{
	//testLexer();
	//testParser();
	//testNFA();
	
	
	
	testMatch();


	
	//cout << "断点" << endl;
}

int main()
{
	test();
	/*std::unordered_map<int, int> umap;
	umap.insert(std::make_pair(1, 2));
	umap.insert(std::make_pair(1, 1));
	umap[1] = 1;
	umap[3] = 4;*/

	system("pause");
	return 0;
}