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
	// �����������ʱ����״ֻ̬���������ߣ������ߵ�˳����Ϊ��ʵ�ַ�̰��ƥ��
	std::reverse(outEdge_.begin(), outEdge_.end());
}

// ��������ʵ�֣���
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
	// ѭ���Ѿ��ﵽ���Ҫ��
	if (!min_)
		exitEdge_->canExit = true;
	// max ֵΪ -1 ��ʾ�������ظ������
	bool result = max_ == -1 || max_ > 0;
	// TODO ����������һ������
	if (max_ > 0) --max_;
	// min_ ����Ϊ 0 �Ͳ���Ҫ�ټ��ˣ��� min_ �ĳ�ʼ��������Ϊ����
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
		cout << "��δ��������" << endl;
		assert(false);
	}

	// first ֵΪ -2 ��ʾ���񵽿�����
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
	// ����ģʽ�� ^ ��ʾ����			$ ��ʾ��β
	// ����ģʽ�� ^ ��ʾ����(\n)֮��	$ ��ʾ��β(\n)֮ǰ
	switch (type_)
	{
	case AnchorType::BEGIN:
		if (automaton->singleLine_ && index == 0) return true;
		if (!automaton->singleLine_ && (index == 0 || content[index] == '\n')) return true;
		return false;
		break;

	case AnchorType::END:
		// TODO ����� size - 1 �����
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