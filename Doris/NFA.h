#ifndef DORIS_NFA_H__
#define DORIS_NFA_H__

#include <vector>
#include <string>
#include <utility>

class NFAEdge;

class NFAState
{
public:
	std::vector<NFAEdge*> inEdge_;
	std::vector<NFAEdge*> outEdge_;
	bool accept_;		// 可以不要吗？是不是只有最后一个是终结的，是的话记住就行了吧？
};

class NFAEdge
{
	char matchContent;
	bool Pass();
	NFAState* start_;
	NFAState* end_;
};

// NFA 回溯时可能需要 reset 一些东西，别如捕获的内容，还有计数边？
std::pair<int, int> RunNFA(int *charClass, NFAState *start, NFAState *end,
	const std::string input);

#endif