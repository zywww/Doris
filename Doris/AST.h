#ifndef DORIS_AST_H__
#define DORIS_AST_H__

#include <tuple>
#include <utility>
#include <vector>
#include <string>
#include "DFA.h"	
#include "NFA.h"

class ASTNode
{
public:
	virtual std::pair<NFAState*, NFAState*> ConstructNFA();
};

class ASTOR : public ASTNode
{
public:
	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	std::vector<ASTNode*> astvec;
};

class ASTCat : public ASTNode
{
private:
	std::vector<ASTNode*> astvec;
};

class ASTRepeat : public ASTNode
{
public:
	ASTNode*	node_;
	bool		greedy_;
	int			min_;
	int			max_;
};

class ASTCharClass : public ASTNode
{
public:
	std::vector<std::pair<int, int>> ranges;
};

class ASTCharacter : public ASTNode
{
public:
	char ch;
};

class ASTBackReference : public ASTNode
{
public: 
	std::string  name;
};

class ASTAnchor : public ASTNode
{
public:
	enum AnchorType { BEGIN, END, BOUND, NOT_BOUND };
	AnchorType type_;
};

class ASTUnnameCapture : public ASTNode
{
public:
	int number;	// 匿名捕获的标号
	ASTNode*	node_;
};

class ASTNameCapture : public ASTNode
{
public:
	std::string name;
	ASTNode*	node_;
};

// positive lookahead
class ASTPstLookahead : public ASTNode
{
public:
	ASTNode*	node_;
};

// negative lookahead
class ASTNgtLookahead : public ASTNode
{
public:
	ASTNode*	node_;
};



/*
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
*/



#endif
