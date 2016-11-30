#include <string>
#include <cassert>
#include <iostream>	
#include <cctype>
#include <algorithm>	
#include "NFA.h"
#include "Automaton.h"
#include "AST.h"

using std::cout;
using std::endl;
using std::string;

void NFAState::ReverseEdgeOrder()
{
	// 调用这个函数时，该状态只有两条出边，调整边的顺序是为了实现非贪婪匹配
	std::reverse(outEdge_.begin(), outEdge_.end());
}

// 用友类来实现？？
NFAEdge::NFAEdge(NFAState* start, NFAState* end) :
	start_(start), end_(end)
{
	start_->outEdge_.push_back(this);
	end_->inEdge_.push_back(this);
}

void NFAEdge::ChangeStartState(NFAState* newStart)
{
	start_ = newStart;
}

bool NFAEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type index)
{
	return true;
}



NFAMatchEdge::NFAMatchEdge(NFAState* start, NFAState* end, char ch) :
	NFAEdge(start, end), ch_(ch)
{
}

bool NFAMatchEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type index)
{
	if (content[index] == ch_)
		return true;
	else
		return false;
}



NFAEmptyEdge::NFAEmptyEdge(NFAState* start, NFAState* end) :
	NFAEdge(start, end)
{
}

bool NFAEmptyEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type index)
{
	return true;
}



NFARepeatEdge::NFARepeatEdge(NFAState* start, NFAState* end, int min, int max, 
	NFAExitEdge* exitEdge) :
	NFAEdge(start, end), min_(min), max_(max), exitEdge_(exitEdge)
{
	assert(min_ >= 0);
	assert(max_ == -1 || max_ >= 0);
}

bool NFARepeatEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type index)
{
	// 循环已经达到最低要求
	if (!min_)
		exitEdge_->canExit = true;
	// max 值为 -1 表示最多可以重复无穷次
	bool result = max_ == -1 || max_ > 0;
	// TODO 这里立即减一合适吗？
	if (max_ > 0) --max_;
	// min_ 减到为 0 就不需要再减了，且 min_ 的初始化不可能为负数
	if (min_ > 0) --min_;
	return result;
}


NFAExitEdge::NFAExitEdge(NFAState* start, NFAState*end) :
	NFAEdge(start, end)
{
}

bool NFAExitEdge::Pass()
{
	return canExit;
}



NFARangeEdge::NFARangeEdge(NFAState* start, NFAState* end, char lhs, char rhs) :
	NFAEdge(start, end), lhs_(lhs), rhs_(rhs)
{
}

bool NFARangeEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type index)
{
	if (content[index] >= lhs_ && content[index] <= rhs_)
		return true;
	else
		return false;
}



NFAReferenceEdge::NFAReferenceEdge(NFAState* start, NFAState* end, string name) :
	NFAEdge(start, end), name_(name)
{
}

bool NFAReferenceEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type index)
{
	auto pair = automaton->GetCaptureContent(name_);
	if (pair.first == -1)
	{
		cout << "尚未捕获到内容" << endl;
		assert(false);
	}

	// first 值为 -2 表示捕获到空内容
	if (pair.first == -2)
		return true;
	if (pair.second - pair.first + 1 > content.size() - index)
		return false;
	for (auto i = pair.first; i <= pair.second; ++i)
		if (content[i] != content[index++])	return false;
	return true;
}



NFAAnchorEdge::NFAAnchorEdge(NFAState* start, NFAState* end, AnchorType type) :
	NFAEdge(start, end), type_(type)
{
	assert(	type_ == AnchorType::BEGIN ||
			type_ == AnchorType::BOUND ||
			type_ == AnchorType::END   ||
			type_ == AnchorType::NOT_BOUND);
}

bool NFAAnchorEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type index)
{
	auto isWord = [](char ch) -> bool
	{
		if (std::isalnum(ch) || ch == '_') return true;
		else return false;
	};
	// 单行模式中 ^ 表示段首			$ 表示段尾
	// 多行模式中 ^ 表示行首(\n)之后	$ 表示行尾(\n)之前
	switch (type_)
	{
	case AnchorType::BEGIN:
		if (automaton->singleLine_ && index == 0) return true;
		if (!automaton->singleLine_ && (index == 0 || content[index] == '\n')) return true;
		return false;
		break;

	case AnchorType::END:
		// TODO 这里的 size - 1 会溢出
		if (automaton->singleLine_ && index + 1 == content.size()) return true;
		if (!automaton->singleLine_ &&
			(index + 1 == content.size() ||
				index + 1 < content.size() && content[index + 1] == '\n'))
			return true;
		return false;
		break;

	case AnchorType::BOUND:
		if (index == 0 || index == content.size()) return true;
		if (isWord(content[index - 1]) ^ isWord(content[index])) return true;
		return false;
		break;

	case AnchorType::NOT_BOUND:
		if (index != 0 && index != content.size() &&
			!((isWord(content[index - 1]) ^ isWord(content[index]))))
				return true;
		return false;
		break;
	}
	
	return true;
}



NFAStartEdge::NFAStartEdge(NFAState* start, NFAState* end, NFAStoreEdge* storeEdge) :
	NFAEdge(start, end), storeEdge_(storeEdge)
{
}

bool NFAStartEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type index)
{
	storeEdge_->SetLhs(index);
	return true;
}



NFAStoreEdge::NFAStoreEdge(NFAState* start, NFAState* end, std::string name) :
	NFAEdge(start, end), name_(name)
{
}

bool NFAStoreEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type index)
{
	automaton->PushPair(name_, lhs_, index);
	return true;
}

void NFAStoreEdge::SetLhs(int lhs)
{
	lhs_ = lhs;
}



NFALookaheadEdge::NFALookaheadEdge(NFAState* start, NFAState* end, bool negate,
	NFAState* lookaheadStart, NFAState* lookaheadEnd) :
	NFAEdge(start, end), negate_(negate),
	lookaheadStart_(lookaheadStart), lookaheadEnd_(lookaheadEnd)
{
}

bool NFALookaheadEdge::Pass()
{
	// TODO
	return true;
}