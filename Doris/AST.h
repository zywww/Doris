#ifndef DORIS_AST_H__
#define DORIS_AST_H__

#include <tuple>
#include <utility>
#include <vector>
#include "DFA.h"	
#include "NFA.h"

class ASTNode
{
public:
};

class ASTGenDFA : ASTNode
{
public:
	bool nullable_;
	std::vector<ASTGenDFA *> firstPos_;
	std::vector<ASTGenDFA *> lastPos_;
	std::vector<ASTGenDFA *> followPos_;
};

class ASTGenNFA : ASTNode
{
public:
	// 这里可不可以不用虚函数
	virtual std::pair<NFAState*, NFAState*> constructNFA();
};

std::tuple<int **, int, int> constructDFA(ASTGenDFA *root);

// 生成字符集映射，如果是 DFA 的话，则需运行这个函数，可以缩小 DFA 转换表的大小
// 返回转换表，长度一般是 256
int* GenCharClass(ASTGenDFA* root);


#endif
