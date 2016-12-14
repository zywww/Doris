#include "Debug.h"
#include <string>
#include <cassert>
#include <iostream>	
#include <cctype>
#include <algorithm>	
#include <vector>
#include "NFA.h"
#include "Automaton.h"
#include "AST.h"


using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;

int NFAState::stateCount = 0;
Automaton* NFAState::automaton = nullptr;
NFAState::NFAState()
{
	automaton->statePool_.push_back(this); 
	++stateCount; 
}
void NFAState::ReverseEdgeOrder()
{
	// 调用这个函数时，该状态只有两条出边，调整边的顺序是为了实现非贪婪匹配
	std::reverse(outEdge_.begin(), outEdge_.end());
}



int NFAEdge::edgeCount = 0;
Automaton* NFAEdge::automaton = nullptr;
// 用友类来实现？？
NFAEdge::NFAEdge(NFAState* start, NFAState* end) :
	start_(start), end_(end)
{
	automaton->edgePool_.push_back(this);
	++edgeCount;
	start_->outEdge_.push_back(this);
	end_->inEdge_.push_back(this);
}

void NFAEdge::ChangeStartState(NFAState* newStart)
{
	start_ = newStart;
	newStart->outEdge_.push_back(this);
}

void NFAEdge::ChangeEndState(NFAState* newEnd)
{
	end_ = newEnd;
	newEnd->inEdge_.push_back(this);
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



NFAStartRepeatEdge::NFAStartRepeatEdge(NFAState* start, NFAState* end, int min, int max) :
	NFAEdge(start, end), min_(min), max_(max)
{
}

void NFAStartRepeatEdge::SetEdges(NFASetRepeatEdge* set, NFARepeatEdge* repeat, NFAExitEdge* exit)
{
	setEdge_ = set;
	repeatEdge_ = repeat;
	exitEdge_	= exit;
}

bool NFAStartRepeatEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	// 重新设置 set 边的 min 和 max，因为如果有重复嵌套的话，比如 (a*)* 该内部重复
	// 会运行多次，所以每次重新运行重复时，由 start 重置 set 边的 min 和 max
	setEdge_->startIndex_ = index;
	setEdge_->min_ = min_;
	setEdge_->max_ = max_;

	// repeat 边 pass 默认为 true，因为一开始总是可以重复的，最后只会由可重复变为不可重复
	// set 边只会设置为 false
	repeatEdge_->pass_ = true;

	// exit 边的 pass 默认初始化为 false，因为一开始总是不可退出的，最后只会由 false 变为 true
	// set 边只会设置为 true
	exitEdge_->pass_ = false;
	return true;
}



NFASetRepeatEdge::NFASetRepeatEdge(NFAState* start, NFAState* end) :
	NFAEdge(start, end)
{
}

bool NFASetRepeatEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	// 如果 max_ 不等于 -1 或者 0 则减少一次
	// max_ = max_ > 0 ? max_ - 1 : max_;
	if (max_ > 0) --max_;
	
	// min_ 减到为 0 就不需要再减了，且 min_ 的初始化不可能为负数
	//min_ = min_ > 0 ? min_ - 1 : min_;
	if (min_ > 0) --min_;


	// 循环已经达到最低要求
	//exitEdge_->pass_ = min_ ? exitEdge_->pass_ : true;
	
	if (!min_)
		exitEdge_->pass_ = true;
	

	// 如果循环无上限并且该次循环匹配内容为空，则不允许再次循环，因为下次循环依然为空
	// 会造成匹配空内容并且无限循环，但是如果是有限次数的，则可以多次匹配空内容
	if (max_ == -1 && index == startIndex_ || max_ == 0)
		repeatEdge_->pass_ = false;

	// 设置重复的位置为当前位置
	startIndex_ = index;
	return true;
}



void NFASetRepeatEdge::SetEdges(NFARepeatEdge* repeat, NFAExitEdge* exit)
{
	repeatEdge_ = repeat;
	exitEdge_ = exit;
}



NFARepeatEdge::NFARepeatEdge(NFAState* start, NFAState* end) :
	NFAEdge(start, end)
{
}

bool NFARepeatEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	return pass_;
}



NFAExitEdge::NFAExitEdge(NFAState* start, NFAState*end) :
	NFAEdge(start, end)
{
}

bool NFAExitEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	return pass_;
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



NFACharClassEdge::NFACharClassEdge(NFAState* start, NFAState* end) :
	NFAEdge(start, end)
{
}

void NFACharClassEdge::SetRanges(const vector<pair<char, char>> &ranges)
{
	ranges_ = ranges;
}

bool NFACharClassEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	if (index >= content.size()) return false;

	auto tempIndex = index++;
	for (auto range : ranges_)
		if (range.first <= content[tempIndex] && content[tempIndex] <= range.second)
			return true;
		
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

	// 若 pair.first == -1 则说明还没捕获到内容
	if (pair.first == -1)
		return true;

	// first 值为 -2 表示捕获到空内容
	if (pair.first == -2)
		return true;

	// 判断未匹配的内容长度是否足够，若不足够则不能通过
	if (pair.second - pair.first > content.size() - index)
		return false;
		
	for (auto i = pair.first; i < pair.second; ++i)
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
		if (automaton->singleLine_ && index == content.size()) return true;
		if (!automaton->singleLine_ &&
			(index == content.size() ||
				index < content.size() && content[index] == '\n'))
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



NFAStoreEdge::NFAStoreEdge(NFAState* start, NFAState* end, const std::string &name) :
	NFAEdge(start, end), 
	name_(name)
{
}

bool NFAStoreEdge::Pass(Automaton* automaton, const std::string& content,
	std::string::size_type &index)
{
	// 即该捕获为空，说明里面的正则表达式匹配的内容为空，则不放进捕获列表
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
	int indexTemp = index;
	int result = automaton->DFSNFAforSearch(lookaheadStart_, content, indexTemp);

	return result == -1 && negate_ || result != -1 && !negate_;
}