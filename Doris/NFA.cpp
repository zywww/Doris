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
	// �����������ʱ����״ֻ̬���������ߣ������ߵ�˳����Ϊ��ʵ�ַ�̰��ƥ��
	std::reverse(outEdge_.begin(), outEdge_.end());
}



int NFAEdge::edgeCount = 0;
Automaton* NFAEdge::automaton = nullptr;
// ��������ʵ�֣���
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
	// �������� set �ߵ� min �� max����Ϊ������ظ�Ƕ�׵Ļ������� (a*)* ���ڲ��ظ�
	// �����ж�Σ�����ÿ�����������ظ�ʱ���� start ���� set �ߵ� min �� max
	setEdge_->startIndex_ = index;
	setEdge_->min_ = min_;
	setEdge_->max_ = max_;

	// repeat �� pass Ĭ��Ϊ true����Ϊһ��ʼ���ǿ����ظ��ģ����ֻ���ɿ��ظ���Ϊ�����ظ�
	// set ��ֻ������Ϊ false
	repeatEdge_->pass_ = true;

	// exit �ߵ� pass Ĭ�ϳ�ʼ��Ϊ false����Ϊһ��ʼ���ǲ����˳��ģ����ֻ���� false ��Ϊ true
	// set ��ֻ������Ϊ true
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
	// ��� max_ ������ -1 ���� 0 �����һ��
	// max_ = max_ > 0 ? max_ - 1 : max_;
	if (max_ > 0) --max_;
	
	// min_ ����Ϊ 0 �Ͳ���Ҫ�ټ��ˣ��� min_ �ĳ�ʼ��������Ϊ����
	//min_ = min_ > 0 ? min_ - 1 : min_;
	if (min_ > 0) --min_;


	// ѭ���Ѿ��ﵽ���Ҫ��
	//exitEdge_->pass_ = min_ ? exitEdge_->pass_ : true;
	
	if (!min_)
		exitEdge_->pass_ = true;
	

	// ���ѭ�������޲��Ҹô�ѭ��ƥ������Ϊ�գ��������ٴ�ѭ������Ϊ�´�ѭ����ȻΪ��
	// �����ƥ������ݲ�������ѭ����������������޴����ģ�����Զ��ƥ�������
	if (max_ == -1 && index == startIndex_ || max_ == 0)
		repeatEdge_->pass_ = false;

	// �����ظ���λ��Ϊ��ǰλ��
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

	// �� pair.first == -1 ��˵����û��������
	if (pair.first == -1)
		return true;

	// first ֵΪ -2 ��ʾ���񵽿�����
	if (pair.first == -2)
		return true;

	// �ж�δƥ������ݳ����Ƿ��㹻�������㹻����ͨ��
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
	// ���ò���Ϊ�գ�˵�������������ʽƥ�������Ϊ�գ��򲻷Ž������б�
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