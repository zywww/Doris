#ifndef DORIS_AST_H__
#define DORIS_AST_H__

enum class AnchorType { BEGIN, END, BOUND, NOT_BOUND };

#include <tuple>
#include <utility>
#include <vector>
#include <string>
///#include "DFA.h"	
//#include "NFA.h"

class NFAState;



// TODO 实现成抽象类？？
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
	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	std::vector<ASTNode*> nodeVec_;
};

class ASTCat : public ASTNode
{
public:
	ASTCat();

	void Push(ASTNode* node);
	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	std::vector<ASTNode*> nodeVec_;
};

class ASTRepeat : public ASTNode
{
public:
	ASTRepeat(ASTNode* node, bool greedy, int min, int max);

	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	node_;
	bool		greedy_;
	int			min_;
	int			max_;
};

class ASTCharClass : public ASTNode
{
public:
	ASTCharClass(bool negate);

	void Push(char begin, char end);
	// charclass 在生成 nfa 时，重新生成最简范围
	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	std::vector<std::pair<char, char>> ranges_;
	bool negate_;
};

class ASTEmpty : public ASTNode
{
public:	
	ASTEmpty() = default;

	std::pair<NFAState*, NFAState*> ConstructNFA();
};

class ASTCharacter : public ASTNode
{
public:
	ASTCharacter(char ch);

	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	char ch_;
};

class ASTBackReference : public ASTNode
{
public: 
	ASTBackReference(int number);

	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	int number_; // 表示 \number 
};

class ASTNameReference : public ASTNode
{
public:
	ASTNameReference(std::string name);

	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	std::string name_;
};


class ASTAnchor : public ASTNode
{
public:
	ASTAnchor(AnchorType type);

	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	AnchorType type_;
};

class ASTUnnameCapture : public ASTNode
{
public:
	ASTUnnameCapture(ASTNode* node, int number);

	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	node_;
	int			number_;	// 匿名捕获的标号
};

class ASTNameCapture : public ASTNode
{
public:
	ASTNameCapture(ASTNode* node, std::string name);

	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	node_;
	std::string name_;
};

// positive lookahead
class ASTPstLookahead : public ASTNode
{
public:
	ASTPstLookahead(ASTNode* node);

	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	node_;
};

// negative lookahead
class ASTNgtLookahead : public ASTNode
{
public:
	ASTNgtLookahead(ASTNode* node);

	std::pair<NFAState*, NFAState*> ConstructNFA();

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
