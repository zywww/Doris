#ifndef DORIS_AUTOMATON_H__
#define DORIS_AUTOMATON_H__

#include <unordered_map>
#include <vector>



class NFAState
{
public:
	std::vector<NFAEdge*> inEdge_;
	std::vector<NFAEdge*> outEdge_;
};

class NFAEdge
{
public:
	NFAEdge(NFAState* start, NFAState* end);
	NFAState*	start_;
	NFAState*	end_;
};

class Automaton
{
public:
	

	// 匿名和命名捕获的内容都存放在这里
	// name begin, end
	std::unordered_map<std::string, std::pair<size_t, size_t>> captureContent_;

	const std::string&		regex = std::string();
	std::string::size_type	index;
};


#endif