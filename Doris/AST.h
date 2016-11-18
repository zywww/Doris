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
	ASTNode();

	virtual std::pair<NFAState*, NFAState*> ConstructNFA();
};

class ASTOR : public ASTNode
{
public:
	ASTOR();

	void Push(ASTNode* node);
	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	std::vector<ASTNode*> nodeVec_;
};

class ASTCat : public ASTNode
{
public:
	ASTCat();

	void Push(ASTNode* node);
	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	std::vector<ASTNode*> nodeVec_;
};

class ASTRepeat : public ASTNode
{
public:
	ASTRepeat(ASTNode* node, bool greedy, int min, int max);

	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	node_;
	bool		greedy_;
	int			min_;
	int			max_;
};

class ASTCharClass : public ASTNode
{
public:
	ASTCharClass();

	void Push(std::pair<char, char> range);
	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	std::vector<std::pair<char, char>> ranges_;
};

class ASTCharacter : public ASTNode
{
public:
	ASTCharacter(char ch);

	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	char ch_;
};

class ASTBackReference : public ASTNode
{
public: 
	ASTBackReference(int number);

	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	int number_; // 表示 \number 
};

enum class AnchorType { BEGIN, END, BOUND, NOT_BOUND };
class ASTAnchor : public ASTNode
{
public:
	ASTAnchor(AnchorType type);

	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	AnchorType type_;
};

class ASTUnnameCapture : public ASTNode
{
public:
	ASTUnnameCapture(ASTNode* node, int number);

	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	node_;
	int			number_;	// 匿名捕获的标号
};

class ASTNameCapture : public ASTNode
{
public:
	ASTNameCapture(ASTNode* node, std::string name);

	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	node_;
	std::string name_;
};

// positive lookahead
class ASTPstLookahead : public ASTNode
{
public:
	ASTPstLookahead(ASTNode* node);

	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	node_;
};

// negative lookahead
class ASTNgtLookahead : public ASTNode
{
public:
	ASTNgtLookahead(ASTNode* node);

	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
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
