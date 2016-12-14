#include <iostream>	
#include <vector>
#include "Automaton.h"
#include "Debug.h"

using std::vector;
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
	std::cout << "gen NFA: " << (double)(end - start) << "ms" << std::endl;
#endif
}
Automaton::~Automaton()
{
	for (auto state : statePool_)
		delete state;
	for (auto edge : edgePool_)
		delete edge;
	NFAEdge::automaton = nullptr;
	NFAState::automaton = nullptr;
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
		//result = DFSNFAforMatchIteration(start_, content);
	
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

	for (auto edge : state->outEdge_)
	{
		size_t indexTemp = index;
		// 调用 pass 的时候，由 pass 函数内部判断指针是否合法，不合法则不能通过
		if (edge->Pass(this, content, indexTemp))
		{
			int result = DFSNFA(edge->end_, content, indexTemp, matchNotGreedy);


			// 如果是匹配，且表达式里有非贪婪匹配，则当匹配到整个表达式才返回匹配到的位置
			// 如果不是匹配，或者表达式中没有非贪婪匹配，也就意味着是搜索或者是完全贪婪的匹配，
			// 则只要找到就返回位置
			if (matchNotGreedy && result == content.size() ||
				!matchNotGreedy && result >= 0)
				return result;			

		}
	}

	return -1;
}


int	Automaton::DFSNFAforMatch(NFAState* state, const std::string& content, int index)
{
	++dfsTime; // 用于统计 该函数调用的次数
	if (state->accept_)  return index;

	// 从该状态出发的每一条边
	for (auto edge : state->outEdge_)
	{
		size_t indexTemp = index;	
		
		// 如果该边可以通过，则递归调用 dfs 该边的结束状态
		if (edge->Pass(this, content, indexTemp))
		{
			int result = DFSNFAforMatch(edge->end_, content, indexTemp);

			// 如果整个表达式都匹配了，则返回结果
			if (result == content.size())
				return result;
		}
	}

	// 如果所有匹配都不成功，则返回 -1
	return -1;
}

int	Automaton::DFSNFAforSearch(NFAState* state, const std::string& content, int index)
{
	++dfsTime; // 用于统计 该函数调用的次数
	if (state->accept_)  return index;

	// 从该状态出发的每一条边
	for (auto edge : state->outEdge_)
	{
		size_t indexTemp = index;

		// 如果该边可以通过，则递归调用 dfs 该边的结束状态
		if (edge->Pass(this, content, indexTemp))
		{
			int result = DFSNFAforSearch(edge->end_, content, indexTemp);

			// 如果搜索到结果，不管长度是多少都返回
			if (result != -1)
				return result;
		}
	}

	// 如果所有匹配都不成功，则返回 -1
	return -1;
}

// 迭代居然比递归慢了 5 倍
int	Automaton::DFSNFAforMatchIteration(NFAState *state, const std::string& content, int index)
{
	vector<pair<NFAState*, size_t>> stk;
	stk.push_back(make_pair(start_, 0));
	//int max = 0;
	while (!stk.empty())
	{
		//max = max > stk.size() ? max : stk.size();
		++dfsTime;
		auto pair = stk.back();
		stk.pop_back();
		if (pair.first->accept_ && pair.second == content.size())
			return pair.second;

		decltype(stk) stkTemp;
		for (auto edge : pair.first->outEdge_)
		{
			size_t indexTemp = pair.second;
			if (edge->Pass(this, content, indexTemp))
				stkTemp.push_back(make_pair(edge->end_, indexTemp));
		}
		
		for (auto i = stkTemp.rbegin(); i != stkTemp.rend(); ++i)
			stk.push_back(*i);
	}
	//std::cout << "stk max size " << max << std::endl;
	return -1;
}