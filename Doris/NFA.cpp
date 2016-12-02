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
	
	// TODO ����������һ������
	if (max_ > 0) --max_;
	// min_ ����Ϊ 0 �Ͳ���Ҫ�ټ��ˣ��� min_ �ĳ�ʼ��������Ϊ����
	if (min_ > 0) --min_;

	// ѭ���Ѿ��ﵽ���Ҫ��
	if (!min_)
		exitEdge_->canExit = true;

	// ���ѭ�������޲��Ҹô�ѭ��ƥ������Ϊ�գ��������ٴ�ѭ������Ϊ�´�ѭ����ȻΪ��
	// �����ƥ������ݲ�������ѭ����������������޴����ģ�����Զ��ƥ�������
	if (max_ == -1 && index == startIndex_)
		return false;
	// �����ظ���λ��Ϊ��ǰλ��
	startIndex_ = index;
	

	// max ֵΪ -1 ��ʾ�������ظ������
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

	// �� pair.first == -1 ��˵����û��������
	// TODO ���ܳ������������ ���﷨�����Ѿ�֤���������˰�
	if (pair.first == -1)
		return true;

	// first ֵΪ -2 ��ʾ���񵽿�����
	if (pair.first == -2)
		return true;
	// �ж�δƥ������ݳ����Ƿ��㹻�������㹻����ͨ��
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
	
	// ���ò���Ϊ�գ�˵�������������ʽƥ�������Ϊ�գ��򲻷Ž������б�
	// �����õ�����Ϊ����ô��
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