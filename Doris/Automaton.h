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

	
	//const std::string&		regex = std::string();
	//std::string::size_type	index;

	// 匿名和命名捕获的内容都存放在这里
	// name begin, end
	std::unordered_map<std::string, std::pair<size_t, size_t>> captureContents_;
	bool					singleLine_ = false;
	NFAState*				start_ = nullptr;
	NFAState*				end_ = nullptr;
};


#endif