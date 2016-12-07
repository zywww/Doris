#include <iostream>	
#include "Automaton.h"
#include "Debug.h"

using std::string;
using std::pair;
using std::make_pair;

Automaton::Automaton(ASTNode* root)
{
#ifdef DORIS_DEBUG
	clock_t start = clock(), end;
	start = clock();
#endif


	NFAState::automaton = this;
	NFAEdge::automaton = this;

	auto pair = root->ConstructNFA();
	pair.second->accept_ = true;
	start_ = pair.first;
	end_ = pair.second;

#ifdef DORIS_DEBUG
	end = clock();
	std::cout << "gen NFA: " << (double)(end - start) << "s" << std::endl;
#endif
}
Automaton::~Automaton()
{
	for (auto state : statePool_)
		delete state;
	for (auto edge : edgePool_)
		delete edge;
}


pair<size_t, size_t> Automaton::GetCaptureContent(string name)
{
	auto p = captureContents_.find(name);
	if (p == captureContents_.end())
		return make_pair(-1, -1);
	return make_pair(p->second.first, p->second.second);
}

void Automaton::PushPair(string name, size_t lhs, size_t rhs)
{
	//std::cout << name << " " <<  lhs << " " << rhs << std::endl;
	captureContents_[name] = make_pair(lhs, rhs);
}

pair<int, int> Automaton::RunNFA(const std::string& content, int startIndex, bool match)
{
	int result;
	if (match)
		result = DFSNFAforMatch(start_, content, startIndex);
	
	//std::cout << dfsTime << std::endl;
	if (result == -1)
		return make_pair(-1, -1);
	else
		return make_pair(startIndex, result);
}

unsigned long long Automaton::dfsTime = 0;
int	Automaton::DFSNFA(NFAState* state, const std::string& content, int index, bool matchNotGreedy)
{
	
	++dfsTime;
	if (state->accept_)  return index; 
	//if (state == end_) return index;
	for (auto edge : state->outEdge_)
	{
		size_t indexTemp = index;
		// 调用 pass 的时候，由 pass 函数内部判断指针是否合法，不合法则不能通过
		if (edge->Pass(this, content, indexTemp))
		{
			int result = DFSNFA(edge->end_, content, indexTemp, matchNotGreedy);
			//std::cout << "matchNotGreedy:" << matchNotGreedy << std::endl;

			// 如果是匹配，且表达式里有非贪婪匹配，则当匹配到整个表达式才返回匹配到的位置
			// 如果不是匹配，或者表达式中没有非贪婪匹配，也就意味着是搜索或者是完全贪婪的匹配，
			// 则只要找到就返回位置
			if (matchNotGreedy && result == content.size() ||
				!matchNotGreedy && result >= 0)
				return result;
			
				
			
			/*
			if (result >= 0)
				return result;
			*/
		}
	}
	return -1;
}

int	Automaton::DFSNFAforMatch(NFAState* state, const std::string& content, int index)
{
	++dfsTime;
	if (state->accept_)  return index;
	//if (state == end_) return index;
	for (auto edge : state->outEdge_)
	{
		size_t indexTemp = index;

		
		// 调用 pass 的时候，由 pass 函数内部判断指针是否合法，不合法则不能通过
		if (edge->Pass(this, content, indexTemp))
		{
			int result = DFSNFAforMatch(edge->end_, content, indexTemp);
			//std::cout << "matchNotGreedy:" << matchNotGreedy << std::endl;

			// 如果是匹配，且表达式里有非贪婪匹配，则当匹配到整个表达式才返回匹配到的位置
			// 如果不是匹配，或者表达式中没有非贪婪匹配，也就意味着是搜索或者是完全贪婪的匹配，
			// 则只要找到就返回位置
			if (result == content.size())
				return result;
		}
		
		
	}
	return -1;
}