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
	std::string::size_type &index)
{
	return true;
}



NFAMatchEdge::NFAMatchEdge(NFAState* start, NFAState* end, char ch) :
	NFAEdge(start, end), ch_(ch)
{
}

bool NFAMatchEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	
	if (index < content.size() && content[index++] == ch_)
		return true;
	else
		return false;
}



NFAEmptyEdge::NFAEmptyEdge(NFAState* start, NFAState* end) :
	NFAEdge(start, end)
{
}

bool NFAEmptyEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	return true;
}



NFAStartRepeatEdge::NFAStartRepeatEdge(NFAState* start, NFAState* end) :
	NFAEdge(start, end)
{
}

void NFAStartRepeatEdge::SetRepeatEdge(NFARepeatEdge* repeatEdge)
{
	repeatEdge_ = repeatEdge;
}

bool NFAStartRepeatEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	repeatEdge_->SetIndex(index);
	exitEdge_->canExit = false;
	return true;
}




NFARepeatEdge::NFARepeatEdge(NFAState* start, NFAState* end, int min, int max, 
	NFAExitEdge* exitEdge) :
	NFAEdge(start, end), min_(min), max_(max), exitEdge_(exitEdge)
{
	exitEdge_->min_ = min_;
	exitEdge_->max_ = max_;
}

bool NFARepeatEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	
	// TODO 这里立即减一合适吗？
	if (max_ > 0) --max_;
	// min_ 减到为 0 就不需要再减了，且 min_ 的初始化不可能为负数
	if (min_ > 0) --min_;

	// 循环已经达到最低要求
	if (!min_)
		exitEdge_->canExit = true;

	// 如果循环无上限并且该次循环匹配内容为空，则不允许再次循环，因为下次循环依然为空
	// 会造成匹配空内容并且无限循环，但是如果是有限次数的，则可以多次匹配空内容
	if (max_ == -1 && index == startIndex_)
		return false;
	// 设置重复的位置为当前位置
	startIndex_ = index;
	

	// max 值为 -1 表示最多可以重复无穷次
	return max_ == -1 || max_ > 0;
}

void NFARepeatEdge::SetIndex(int index)
{
	startIndex_ = index;
}


NFAExitEdge::NFAExitEdge(NFAState* start, NFAState*end) :
	NFAEdge(start, end)
{
}

bool NFAExitEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	repeatEdge_->min_ = min_;
	repeatEdge_->max_ = max_;
	return canExit;
}

void NFAExitEdge::SetRepeatEdge(NFARepeatEdge* repeatEdge)
{
	repeatEdge_ = repeatEdge;
}



NFARangeEdge::NFARangeEdge(NFAState* start, NFAState* end, char lhs, char rhs) :
	NFAEdge(start, end), lhs_(lhs), rhs_(rhs)
{
}

bool NFARangeEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	auto tempIndex = index++;
	if (tempIndex < content.size() &&  lhs_ <= content[tempIndex] && content[tempIndex] <= rhs_)
		return true;
	else
		return false;
}



NFAReferenceEdge::NFAReferenceEdge(NFAState* start, NFAState* end, string name) :
	NFAEdge(start, end), name_(name)
{
}

bool NFAReferenceEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	auto pair = automaton->GetCaptureContent(name_);
	std::cout << "pass func " << pair.first << " " << pair.second <<
		" index = " << index << endl;

	// 若 pair.first == -1 则说明还没捕获到内容
	// TODO 可能出现这种情况吗 在语法分析已经证明不可能了把
	if (pair.first == -1)
		return true;

	// first 值为 -2 表示捕获到空内容
	if (pair.first == -2)
		return true;
	// 判断未匹配的内容长度是否足够，若不足够则不能通过
	if (pair.second - pair.first > content.size() - index)
		return false;
		
	
	for (auto i = pair.first; i <= pair.second; ++i)
		if (content[i] != content[index++])	return false;
	return true;
}



NFAAnchorEdge::NFAAnchorEdge(NFAState* start, NFAState* end, AnchorType type) :
	NFAEdge(start, end), type_(type)
{
}

bool NFAAnchorEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
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
	std::string::size_type &index)
{
	storeEdge_->SetLhs(index);
	return true;
}



NFAStoreEdge::NFAStoreEdge(NFAState* start, NFAState* end, std::string name) :
	NFAEdge(start, end), name_(name)
{
}

bool NFAStoreEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	//cout << name_ << endl;
	
	// 即该捕获为空，说明里面的正则表达式匹配的内容为空，则不放进捕获列表
	// 那引用的内容为空怎么办
	if (lhs_ != index)
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

bool NFALookaheadEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	// TODO
	return true;
}