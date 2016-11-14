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
	bool accept_;		// ���Բ�Ҫ���ǲ���ֻ�����һ�����ս�ģ��ǵĻ���ס�����˰ɣ�
};

class NFAEdge
{
	char matchContent;
	bool Pass();
	NFAState* start_;
	NFAState* end_;
};

// NFA ����ʱ������Ҫ reset һЩ���������粶������ݣ����м����ߣ�
std::pair<int, int> RunNFA(int *charClass, NFAState *start, NFAState *end,
	const std::string input);

#endif