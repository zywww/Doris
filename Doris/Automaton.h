#ifndef DORIS_AUTOMATON_H__
#define DORIS_AUTOMATON_H__

#include <unordered_map>
#include <vector>
#include <utility>
#include <cstddef>
#include <string>
#include "NFA.h"
#include "AST.h"


class Automaton
{
public:
	Automaton(ASTNode* root);
	~Automaton();

	std::pair<size_t, size_t>	GetCaptureContent(std::string name);
	void						PushPair(std::string name, size_t lhs, size_t rhs);

	// ���ҵ��򷵻����䣬���򷵻� -1��-1
	std::pair<int, int>	RunNFA(const std::string& content, int startIndex = 0, 
		bool matchNotGreedy = false);
	// �����Դﵽ����״̬���򷵻� index�����򷵻� -1
	int					DFSNFA(NFAState* start, const std::string& content, int startIndex = 0,
		bool matchNotGreedy = false);


	// ������������������ݶ����������
	// name begin, end
	std::unordered_map<std::string, std::pair<size_t, size_t>> captureContents_;
	bool					singleLine_ = false;
	NFAState*				start_ = nullptr;
	NFAState*				end_ = nullptr;
	std::vector<NFAState*>	statePool_;
	std::vector<NFAEdge*>	edgePool_;
};


#endif