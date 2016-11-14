#ifndef DORIS_DFA_H__
#define DORIS_DFA_H__

#include <vector>
#include <string>
#include <utility>

class DFAEdge;

class DFAState
{
	std::vector<DFAEdge*> inEdge_;	// ���Բ��� inEdge_ ��
	std::vector<DFAEdge*> outEdge_;
	bool accept_;
};

class DFAEdge
{
public:
	char matchContent_;
	DFAState *start_;
	DFAState *end_;
};



// DFA ����Ҫ�����ַ����ϣ���ʹ�ñ�������
std::pair<int, int> RunDFA(int *charClass, int **DFA, const std::string input);

int** optimize(int** DFA); // �Ż� DFA 

#endif