#ifndef DORIS_NFA_H__
#define DORIS_NFA_H__

//enum class AnchorType { BEGIN, END, BOUND, NOT_BOUND };

#include <vector>
#include <string>
#include <utility>


enum class AnchorType;
class NFAEdge;
class Automaton;



class NFAState
{
public:
	std::vector<NFAEdge*> inEdge_;
	std::vector<NFAEdge*> outEdge_;
	void ReverseEdgeOrder();
	bool				  accept_ = false;
};

class NFAEdge
{
public:
	NFAEdge(NFAState* start, NFAState* end);

	void ChangeStartState(NFAState* newStart);
	virtual bool Pass(Automaton* automaton, const std::string& content, 
		std::string::size_type index);

private:
	NFAState*	start_;
	NFAState*	end_;
};

class NFAMatchEdge : public NFAEdge
{
public:
	NFAMatchEdge(NFAState* start, NFAState* end, char ch);
	
	bool Pass(Automaton* automaton, const std::string& content, 
		std::string::size_type index);

private:
	char ch_;
};

class NFAEmptyEdge : public NFAEdge
{
public:
	NFAEmptyEdge(NFAState* start, NFAState* end);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type index);
};

class NFAExitEdge;

class NFARepeatEdge : public NFAEdge
{
public:
	NFARepeatEdge(NFAState* start, NFAState* end, int min, int max, NFAExitEdge* exitEdge);
	
	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type index);

private:
	int				min_;
	int				max_;
	NFAExitEdge*	exitEdge_;
};

class NFAExitEdge : public NFAEdge
{
public:
	NFAExitEdge(NFAState* start, NFAState* end);

	bool Pass();

	bool canExit = false;
};

class NFARangeEdge : public NFAEdge
{
public:
	NFARangeEdge(NFAState* start, NFAState* end, char lhs, char rhs);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type index);

private:
	char lhs_;
	char rhs_;
};

class NFAReferenceEdge : public NFAEdge
{
public:
	NFAReferenceEdge(NFAState* start, NFAState* end, std::string name);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type index);

private:
	std::string name_;
};

class NFAAnchorEdge : public NFAEdge
{
public:
	NFAAnchorEdge(NFAState* start, NFAState* end, AnchorType type);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type index);

private:
	AnchorType type_;
};

class NFAStoreEdge;

class NFAStartEdge : public NFAEdge	
{
public:
	NFAStartEdge(NFAState* start, NFAState* end, NFAStoreEdge* storeEdge);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type index);

private:
	NFAStoreEdge* storeEdge_;
};

class NFAStoreEdge : public NFAEdge
{
public:
	NFAStoreEdge(NFAState* start, NFAState* end, std::string name);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type index);
	void SetLhs(int lhs);

private:
	std::string name_;
	int			lhs_;
	//int			rhs_;
};

class NFALookaheadEdge : public NFAEdge
{
public:
	NFALookaheadEdge(NFAState* start, NFAState* end, bool negate,
		NFAState* lookaheadStart, NFAState* lookaheadEnd);

	bool Pass();

private:
	bool negate_;
	NFAState* lookaheadStart_;
	NFAState* lookaheadEnd_;
};


// NFA 回溯时可能需要 reset 一些东西，别如捕获的内容，还有计数边？
std::pair<int, int> RunNFA(int *charClass, NFAState *start, NFAState *end,
	const std::string input);

#endif