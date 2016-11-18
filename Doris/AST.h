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
	virtual std::pair<NFAState*, NFAState*> ConstructNFA();
};

class ASTOR : public ASTNode
{
public:
	virtual std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	left_;
	ASTNode*	right_;
};

class ASTCat : public ASTNode
{

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
	char begin;
	char end;
};

class ASTCharacter : public ASTNode
{
public:
	char ch;
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
