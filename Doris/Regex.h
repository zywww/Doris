#ifndef DORIS_REGEX_H__
#define DORIS_REGEX_H__

#include <string>
#include "NFA.h"
#include "DFA.h"
#include "Parser.h"
#include "Automaton.h"

// 要销毁的资源有 AST 和 自动机 的资源
class Regex
{
public:
	// 可以在构造函数中编译正则表达式，因为一个 regex 类表示一个正则表达式
	Regex(const std::string &regex, const std::string &matchContent);

	// match 先调用 parse 再调用 find 获得第一个搜索到的位置，
	bool				Match(const std::string &matchContent);
	bool				Search();	// 暂时不用
	bool				Replace();	// 暂时不用

private:
	const std::string	regex_;
	const std::string   matchContent_;
	Parser				parser_;
	Automaton			automaton_;
	bool				isDFA_;


	bool				Parse();
	std::pair<int, int>	Find(int begin, const std::string &matchContent, bool match); 
};

#endif 