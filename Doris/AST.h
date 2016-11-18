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
	int number_; // ��ʾ \number 
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
	int			number_;	// ��������ı��
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
	// ����ɲ����Բ����麯��
	virtual std::pair<NFAState*, NFAState*> constructNFA();
};

std::tuple<int **, int, int> constructDFA(ASTGenDFA *root);

// �����ַ���ӳ�䣬����� DFA �Ļ��������������������������С DFA ת����Ĵ�С
// ����ת��������һ���� 256
int* GenCharClass(ASTGenDFA* root);
*/



#endif
