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

	// 调用方法前，先判断是否 Parse，若未 Parse，则调用 Parse 方法
	// Parse 方法调用语法分析器的 Parse，得到 AST 根结点，
	// 然后根据语法分析器的标志判断是生成 DFA 还是 NFA，然后获得对应的
	// 自动机结点，接口方法通过运行自动机完成对应功能
	bool				Match(const std::string &matchContent);
	bool				Search();
	bool				Replace();

private:
	const std::string	regex_;
	bool				parse_;
	NFAState*			NFAStart;
	DFAState*			DFAStart;

	bool				Parse();
};

#endif 