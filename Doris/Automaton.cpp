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
		// ���� pass ��ʱ���� pass �����ڲ��ж�ָ���Ƿ�Ϸ������Ϸ�����ͨ��
		if (edge->Pass(this, content, indexTemp))
		{
			int result = DFSNFA(edge->end_, content, indexTemp, matchNotGreedy);


			// �����ƥ�䣬�ұ��ʽ���з�̰��ƥ�䣬��ƥ�䵽�������ʽ�ŷ���ƥ�䵽��λ��
			// �������ƥ�䣬���߱��ʽ��û�з�̰��ƥ�䣬Ҳ����ζ����������������ȫ̰����ƥ�䣬
			// ��ֻҪ�ҵ��ͷ���λ��
			if (matchNotGreedy && result == content.size() ||
				!matchNotGreedy && result >= 0)
				return result;			

		}
	}

	return -1;
}


int	Automaton::DFSNFAforMatch(NFAState* state, const std::string& content, int index)
{
	++dfsTime; // ����ͳ�� �ú������õĴ���
	if (state->accept_)  return index;

	// �Ӹ�״̬������ÿһ����
	for (auto edge : state->outEdge_)
	{
		size_t indexTemp = index;	
		
		// ����ñ߿���ͨ������ݹ���� dfs �ñߵĽ���״̬
		if (edge->Pass(this, content, indexTemp))
		{
			int result = DFSNFAforMatch(edge->end_, content, indexTemp);

			// ����������ʽ��ƥ���ˣ��򷵻ؽ��
			if (result == content.size())
				return result;
		}
	}

	// �������ƥ�䶼���ɹ����򷵻� -1
	return -1;
}

int	Automaton::DFSNFAforSearch(NFAState* state, const std::string& content, int index)
{
	++dfsTime; // ����ͳ�� �ú������õĴ���
	if (state->accept_)  return index;

	// �Ӹ�״̬������ÿһ����
	for (auto edge : state->outEdge_)
	{
		size_t indexTemp = index;

		// ����ñ߿���ͨ������ݹ���� dfs �ñߵĽ���״̬
		if (edge->Pass(this, content, indexTemp))
		{
			int result = DFSNFAforSearch(edge->end_, content, indexTemp);

			// �����������������ܳ����Ƕ��ٶ�����
			if (result != -1)
				return result;
		}
	}

	// �������ƥ�䶼���ɹ����򷵻� -1
	return -1;
}

// ������Ȼ�ȵݹ����� 5 ��
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