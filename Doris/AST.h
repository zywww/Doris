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
	int number;	// ��������ı��
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
	// ����ɲ����Բ����麯��
	virtual std::pair<NFAState*, NFAState*> constructNFA();
};

std::tuple<int **, int, int> constructDFA(ASTGenDFA *root);

// �����ַ���ӳ�䣬����� DFA �Ļ��������������������������С DFA ת����Ĵ�С
// ����ת��������һ���� 256
int* GenCharClass(ASTGenDFA* root);
*/



#endif
