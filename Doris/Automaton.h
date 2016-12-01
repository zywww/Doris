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

	std::pair<size_t, size_t>	GetCaptureContent(std::string name);
	void						PushPair(std::string name, size_t lhs, size_t rhs);

	// ���ҵ��򷵻����䣬���򷵻� -1��-1
	std::pair<int, int>	RunNFA(const std::string& content, int startIndex = 0);
	// �����Դﵽ����״̬���򷵻� index�����򷵻� -1
	int					DFSNFA(NFAState* start, const std::string& content, int startIndex = 0);

	
	//const std::string&		regex = std::string();
	//std::string::size_type	index;

	// ������������������ݶ����������
	// name begin, end
	std::unordered_map<std::string, std::pair<size_t, size_t>> captureContents_;
	bool					singleLine_ = false;
	NFAState*				start_ = nullptr;
	NFAState*				end_ = nullptr;
};


#endif