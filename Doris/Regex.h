#ifndef DORIS_REGEX_H__
#define DORIS_REGEX_H__

#include <string>
#include "NFA.h"
#include "DFA.h"
#include "Parser.h"

// 要销毁的资源有 AST 和 自动机 的资源
class Regex
{
public:
	Regex(const std::string regex);

	// match 先调用 parse 再调用 find 获得第一个搜索到的位置，
	bool				Match(const std::string &matchContent);
	bool				Search();	// 暂时不用
	bool				Replace();	// 暂时不用

private:
	const std::string	regex_;
	Parser				parser_;
	bool				hasParse_;
	NFAState*			NFAStart_;
	DFAState*			DFAStart_;
	bool				isDFA_;

	// 调用 parse_ 的 parse 方法，获得 AST 根结点，然后根据 parse_ 的标志判断是否可以生成 DFA，
	// 然后调用 AST 根节点对应的构造自动机方法，并设置 dfaStart 成员，
	bool				Parse();
	// 在接口中调用此方法，此方法返回字符串中搜索到的第一个位置, 
	// 根据 match 标志判断是否是匹配操作，若是匹配，则自动机必须运行一定的长度
	// 会根据 isDFA_ 来选择调用 rundfa 或者 runnfa ？
	// 找不到会返回 -1
	std::pair<int, int>	Find(int begin, const std::string &matchContent, bool match); 
};

#endif 