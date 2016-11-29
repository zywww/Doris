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
	captureContents_.insert(make_pair(name, make_pair(lhs, rhs)));
}