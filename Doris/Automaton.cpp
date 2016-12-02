#include <iostream>	
#include "Automaton.h"

using std::string;
using std::pair;
using std::make_pair;

Automaton::Automaton(ASTNode* root)
{
	auto pair = root->ConstructNFA();
	pair.second->accept_ = true;
	start_ = pair.first;
	end_ = pair.second;
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
	std::cout << lhs << " " << rhs << std::endl;
	captureContents_.insert(make_pair(name, make_pair(lhs, rhs)));
}

pair<int, int> Automaton::RunNFA(const std::string& content, int startIndex)
{
	int result = DFSNFA(start_, content, startIndex);
	if (result == -1)
		return make_pair(-1, -1);
	else
		return make_pair(startIndex, result);
}

int	Automaton::DFSNFA(NFAState* state, const std::string& content, int index)
{
	if (state->accept_) return index;
	
	for (auto edge : state->outEdge_)
	{
		size_t indexTemp = index;
		// 调用 pass 的时候，由 pass 函数内部判断指针是否合法，不合法则不能通过
		if (edge->Pass(this, content, indexTemp))
		{
			int result = DFSNFA(edge->end_, content, indexTemp);
			if (result >= 0)
				return result;
		}
	}
	return -1;
}