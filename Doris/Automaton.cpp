#include "Automaton.h"

// ��������ʵ�֣���
NFAEdge::NFAEdge(NFAState* start, NFAState* end) :
	start_(start), end_(end)
{
	start_->outEdge_.push_back(this);
	end_->inEdge_.push_back(this);
}

