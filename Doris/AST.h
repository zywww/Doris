#ifndef DORIS_AST_H__
#define DORIS_AST_H__



#include <tuple>
#include <utility>
#include <vector>
#include <string>

class NFAState;
enum class AnchorType { BEGIN, END, BOUND, NOT_BOUND };

// TODO ʵ�ֳɳ����ࣿ��
class ASTNode
{
public:
	ASTNode();
	virtual ~ASTNode();

	virtual std::pair<NFAState*, NFAState*> ConstructNFA();
};


class ASTOR : public ASTNode
{
public:
	ASTOR();
	~ASTOR();

	// �� �� ���ӽڵ�����
	void							Push(ASTNode* node);
	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	std::vector<ASTNode*> nodeVec_;
};


class ASTCat : public ASTNode
{
public:
	ASTCat();
	~ASTCat();

	// �� ���� ���ӽڵ�����
	void							Push(ASTNode* node);
	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	std::vector<ASTNode*> nodeVec_;
};


class ASTRepeat : public ASTNode
{
public:
	ASTRepeat(ASTNode* node, bool greedy, int min, int max);
	~ASTRepeat();

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

	// ��һϵ�е��ַ�����ӽ���
	void							Push(char begin, char end);
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
	int number_; // ��ʾ \number 
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
	~ASTUnnameCapture();

	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	node_;
	int			number_;	// ��������ı��
};


class ASTNameCapture : public ASTNode
{
public:
	ASTNameCapture(ASTNode* node, std::string name);
	~ASTNameCapture();

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
	~ASTPstLookahead();

	std::pair<NFAState*, NFAState*> ConstructNFA();

private:
	ASTNode*	node_;
};


// negative lookahead
class ASTNgtLookahead : public ASTNode
{
public:
	ASTNgtLookahead(ASTNode* node);
	~ASTNgtLookahead();

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
	// ����ɲ����Բ����麯��
	virtual std::pair<NFAState*, NFAState*> constructNFA();
};

std::tuple<int **, int, int> constructDFA(ASTGenDFA *root);

// �����ַ���ӳ�䣬����� DFA �Ļ��������������������������С DFA ת����Ĵ�С
// ����ת��������һ���� 256
int* GenCharClass(ASTGenDFA* root);
*/



#endif
