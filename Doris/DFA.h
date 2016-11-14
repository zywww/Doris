#ifndef DORIS_DFA_H__
#define DORIS_DFA_H__

#include <vector>
#include <string>
#include <utility>

class DFAEdge;

class DFAState
{
	std::vector<DFAEdge*> inEdge_;	// 可以不用 inEdge_ 吗？
	std::vector<DFAEdge*> outEdge_;
	bool accept_;
};

class DFAEdge
{
public:
	char matchContent_;
	DFAState *start_;
	DFAState *end_;
};



// DFA 才需要构造字符集合，才使用表来运行
std::pair<int, int> RunDFA(int *charClass, int **DFA, const std::string input);

int** optimize(int** DFA); // 优化 DFA 

#endif