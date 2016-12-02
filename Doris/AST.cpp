#include <utility>	
#include <string>
#include <cassert>
#include <algorithm>
#include <iostream>
#include "AST.h"
#include "NFA.h"

using std::string;
using std::pair;
using std::make_pair;

ASTNode::ASTNode()
{
}

pair<NFAState*, NFAState*> ASTNode::ConstructNFA()
{
	return make_pair(nullptr, nullptr);
}


ASTOR::ASTOR()
{
}

void ASTOR::Push(ASTNode* node)
{
	nodeVec_.push_back(node);
}

std::pair<NFAState*, NFAState*> ASTOR::ConstructNFA()
{
	assert(nodeVec_.size() != 0);
	if (nodeVec_.size() == 1) return nodeVec_[0]->ConstructNFA();
	NFAState* start = new NFAState;
	NFAState* end = new NFAState;
	for (auto node : nodeVec_)
	{
		auto pair = node->ConstructNFA();
		new NFAEmptyEdge(start, pair.first);
		new NFAEmptyEdge(pair.second, end);
	}
	return make_pair(start, end);
}


ASTCat::ASTCat()
{
}

void ASTCat::Push(ASTNode* node)
{
	nodeVec_.push_back(node);
}

// 可以证明每个结构生成的 NFA 的 start 状态只有出边，end 状态只有入边
std::pair<NFAState*, NFAState*> ASTCat::ConstructNFA()
{
	assert(nodeVec_.size() != 0);
	
	auto pair = nodeVec_[0]->ConstructNFA();
	auto start = pair.first;
	auto end = pair.second;
	for (int i = 1; i < nodeVec_.size(); ++i)
	{
		auto tempPair = nodeVec_[i]->ConstructNFA();
		for (auto outEdge : tempPair.first->outEdge_)
		{
			outEdge->ChangeStartState(end);
			end->outEdge_.push_back(outEdge);
		}
			
		// delete tempPair.first; // 释放内存？
		end = tempPair.second;
	}
	return make_pair(start, end);
}

ASTRepeat::ASTRepeat(ASTNode* node, bool greedy, int min, int max) :
	node_(node), greedy_(greedy), min_(min), max_(max)
{
}

std::pair<NFAState*, NFAState*> ASTRepeat::ConstructNFA()
{
	auto pair = node_->ConstructNFA();
	auto start = new NFAState;
	auto end = new NFAState;
	// 如果重复的最大次数为 0，那么可以优化为空边
	if (!max_)
	{
		new NFAEmptyEdge(start, end);
		return make_pair(start, end);
	}

	/*
	auto exitEdge = new NFAExitEdge(pair.second, end);
	if (greedy_)
	{
		new NFARepeatEdge(pair.second, pair.first, min_, max_, exitEdge);
		pair.second->ReverseEdgeOrder();
		new NFAEmptyEdge(start, pair.first);
		if (!min_)
			new NFAEmptyEdge(start, end);
	}
	else
	{
		new NFARepeatEdge(pair.second, pair.first, min_, max_, exitEdge);
		if (!min_)
			new NFAEmptyEdge(start, end);
		new NFAEmptyEdge(start, pair.first);
	}
	*/

	auto exitEdge = new NFAExitEdge(pair.second, end);
	auto startEdge = new NFAStartRepeatEdge(start, pair.first);
	if (greedy_)
	{
		auto repeatEdge = new NFARepeatEdge(pair.second, pair.first, min_, max_, exitEdge);
		startEdge->repeatEdge_ = repeatEdge;
		startEdge->exitEdge_ = exitEdge;
		exitEdge->SetRepeatEdge(repeatEdge);
		pair.second->ReverseEdgeOrder();
		if (!min_)
			new NFAEmptyEdge(start, end);
	}
	else
	{
		auto repeatEdge = new NFARepeatEdge(pair.second, pair.first, min_, max_, exitEdge);
		startEdge->repeatEdge_ = repeatEdge;
		startEdge->exitEdge_ = exitEdge;
		exitEdge->SetRepeatEdge(repeatEdge);
		if (!min_)
			new NFAEmptyEdge(start, end);
	}

	return make_pair(start, end);
}


ASTCharClass::ASTCharClass(bool negate) : 
	negate_(negate)
{
}

void ASTCharClass::Push(char begin, char end)
{
	ranges_.push_back(std::make_pair(begin, end));
}

std::pair<NFAState*, NFAState*> ASTCharClass::ConstructNFA()
{
	// TODO 对于 negate 的实现
	// TODO 可以优化
	auto start = new NFAState;
	auto end = new NFAState;
	if (ranges_.size() == 1)
		new NFARangeEdge(start, end, ranges_[0].first, ranges_[0].second);
	else
	{
		for (auto pair : ranges_)
		{
			auto tempStart = new NFAState;
			auto tempEnd = new NFAState;
			new NFAEmptyEdge(start, tempStart);
			new NFAEmptyEdge(tempEnd, end);
			new NFARangeEdge(tempStart, tempEnd, pair.first, pair.second);
		}
	}
	return make_pair(start, end);
}

std::pair<NFAState*, NFAState*> ASTEmpty::ConstructNFA()
{
	auto start = new NFAState;
	auto end = new NFAState;
	new NFAEmptyEdge(start, end);
	return make_pair(start, end);
}


ASTCharacter::ASTCharacter(char ch) : 
	ch_(ch)
{
}

std::pair<NFAState*, NFAState*> ASTCharacter::ConstructNFA()
{
	auto start = new NFAState;
	auto end = new NFAState;
	new NFAMatchEdge(start, end, ch_);
	return make_pair(start, end);
}


ASTBackReference::ASTBackReference(int number) : 
	number_(number)
{
}

std::pair<NFAState*, NFAState*> ASTBackReference::ConstructNFA()
{
	string name;
	int number = number_;
	while (number)
	{
		name += number % 10 + '0';
		number /= 10;
	}
	std::reverse(name.begin(), name.end());
	auto start = new NFAState;
	auto end = new NFAState;
	new NFAReferenceEdge(start, end, name);
	return make_pair(start, end);
}


ASTNameReference::ASTNameReference(string name) :
	name_(name)
{
}

std::pair<NFAState*, NFAState*> ASTNameReference::ConstructNFA()
{
	auto start = new NFAState;
	auto end = new NFAState;
	new NFAReferenceEdge(start, end, name_);
	return make_pair(start, end);
}


ASTAnchor::ASTAnchor(AnchorType type) : 
	type_(type)
{
}

std::pair<NFAState*, NFAState*> ASTAnchor::ConstructNFA()
{
	auto start = new NFAState;
	auto end = new NFAState;
	new NFAAnchorEdge(start, end, type_);
	return make_pair(start, end);
}


ASTUnnameCapture::ASTUnnameCapture(ASTNode* node, int number) :
	node_(node), number_(number)
{
}

std::pair<NFAState*, NFAState*> ASTUnnameCapture::ConstructNFA()
{
	// TODO 代码重复？？
	string name;
	int number = number_;
	while (number)
	{
		name += number % 10 + '0';
		number /= 10;
	}
	std::reverse(name.begin(), name.end());
	

	auto start = new NFAState;
	auto end = new NFAState;
	auto pair = node_->ConstructNFA();
	auto storeEdge = new NFAStoreEdge(pair.second, end, name);
	new NFAStartEdge(start, pair.first, storeEdge);
	return make_pair(start, end);
}

ASTNameCapture::ASTNameCapture(ASTNode* node, string name) :
	node_(node), name_(name)
{
}

std::pair<NFAState*, NFAState*> ASTNameCapture::ConstructNFA()
{
	auto start = new NFAState;
	auto end = new NFAState;
	auto pair = node_->ConstructNFA();
	auto storeEdge = new NFAStoreEdge(pair.second, end, name_);
	new NFAStartEdge(start, pair.first, storeEdge);
	return make_pair(start, end);
}



ASTPstLookahead::ASTPstLookahead(ASTNode* node) :
	node_(node)
{
}

std::pair<NFAState*, NFAState*> ASTPstLookahead::ConstructNFA()
{
	auto start = new NFAState;
	auto end = new NFAState;
	auto pair = node_->ConstructNFA();
	new NFALookaheadEdge(start, end, false, pair.first, pair.second);
	return make_pair(start, end);
}


ASTNgtLookahead::ASTNgtLookahead(ASTNode* node) :
	node_(node)
{
}

std::pair<NFAState*, NFAState*> ASTNgtLookahead::ConstructNFA()
{
	auto start = new NFAState;
	auto end = new NFAState;
	auto pair = node_->ConstructNFA();
	new NFALookaheadEdge(start, end, true, pair.first, pair.second);
	return make_pair(start, end);
}