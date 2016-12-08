#ifndef DORIS_NFA_H__
#define DORIS_NFA_H__

#include <vector>
#include <string>
#include <utility>


enum class AnchorType;
class NFAEdge;
class Automaton;
class NFAExitEdge;
class NFARepeatEdge;
class NFASetRepeatEdge;
class NFAStoreEdge;

class NFAState
{
public:
	static Automaton* automaton;
	static int stateCount;

	NFAState();
	void ReverseEdgeOrder();

	std::vector<NFAEdge*> inEdge_;
	std::vector<NFAEdge*> outEdge_;
	bool				  accept_ = false;
};


class NFAEdge
{
public:
	static Automaton* automaton;
	static int edgeCount;

	NFAEdge(NFAState* start, NFAState* end);

	// 用于连接时合并两个状态
	void		 ChangeStartState(NFAState* newStart);
	void		 ChangeEndState(NFAState* newEnd);
	virtual bool Pass(Automaton* automaton, const std::string& content, 
		std::string::size_type &index);

	NFAState*	start_;
	NFAState*	end_;
};


class NFAMatchEdge : public NFAEdge
{
public:
	NFAMatchEdge(NFAState* start, NFAState* end, char ch);
	
	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);

	char ch_;
};


class NFAEmptyEdge : public NFAEdge
{
public:
	NFAEmptyEdge(NFAState* start, NFAState* end);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);
};


class NFAStartRepeatEdge : public NFAEdge
{
public:
	NFAStartRepeatEdge(NFAState* start, NFAState* end, int min, int max);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);
	void SetEdges(NFASetRepeatEdge* set, NFARepeatEdge* repeat, NFAExitEdge* exit);

	int min_;
	int max_;
	NFASetRepeatEdge*	setEdge_;
	NFARepeatEdge*		repeatEdge_;
	NFAExitEdge*		exitEdge_;
};


class NFASetRepeatEdge : public NFAEdge
{
public:
	NFASetRepeatEdge(NFAState* start, NFAState* end);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);
	void SetEdges(NFARepeatEdge* repeat, NFAExitEdge* exit);

	int min_;
	int max_;
	int startIndex_;

	NFARepeatEdge*	repeatEdge_;
	NFAExitEdge*	exitEdge_;
};


class NFARepeatEdge : public NFAEdge
{
public:
	NFARepeatEdge(NFAState* start, NFAState* end);
	
	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);

	bool pass_;
};


class NFAExitEdge : public NFAEdge
{
public:
	NFAExitEdge(NFAState* start, NFAState* end);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);

	bool pass_;
};


class NFARangeEdge : public NFAEdge
{
public:
	NFARangeEdge(NFAState* start, NFAState* end, char lhs, char rhs);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);

	char lhs_;
	char rhs_;
};

class NFACharClassEdge : public NFAEdge
{
public:
	NFACharClassEdge(NFAState* start, NFAState* end);

	void SetRanges(const std::vector<std::pair<char, char>> &range);
	bool Pass(Automaton* automaton, const std::string& content,
			std::string::size_type &index);

	std::vector<std::pair<char, char>> ranges_;
};


class NFAReferenceEdge : public NFAEdge
{
public:
	NFAReferenceEdge(NFAState* start, NFAState* end, std::string name);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);

	std::string name_;
};


class NFAAnchorEdge : public NFAEdge
{
public:
	NFAAnchorEdge(NFAState* start, NFAState* end, AnchorType type);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);

	AnchorType type_;
};


class NFAStartEdge : public NFAEdge	
{
public:
	NFAStartEdge(NFAState* start, NFAState* end, NFAStoreEdge* storeEdge);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);

	NFAStoreEdge* storeEdge_;
};


class NFAStoreEdge : public NFAEdge
{
public:
	NFAStoreEdge(NFAState* start, NFAState* end, const std::string &name);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);
	void SetLhs(int lhs);

	std::string name_;
	int			lhs_;
};

class NFALookaheadEdge : public NFAEdge
{
public:
	NFALookaheadEdge(NFAState* start, NFAState* end, bool negate,
		NFAState* lookaheadStart, NFAState* lookaheadEnd);

	bool Pass(Automaton* automaton, const std::string& content,
		std::string::size_type &index);

	bool negate_;
	NFAState* lookaheadStart_;
	NFAState* lookaheadEnd_;
};

#endif