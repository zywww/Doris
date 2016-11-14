#ifndef DORIS_REGEX_H__
#define DORIS_REGEX_H__

#include <string>
#include "NFA.h"
#include "DFA.h"
#include "Parser.h"

// Ҫ���ٵ���Դ�� AST �� �Զ��� ����Դ
class Regex
{
public:
	Regex(const std::string regex);

	// match �ȵ��� parse �ٵ��� find ��õ�һ����������λ�ã�
	bool				Match(const std::string &matchContent);
	bool				Search();	// ��ʱ����
	bool				Replace();	// ��ʱ����

private:
	const std::string	regex_;
	Parser				parser_;
	bool				hasParse_;
	NFAState*			NFAStart_;
	DFAState*			DFAStart_;
	bool				isDFA_;

	// ���� parse_ �� parse ��������� AST ����㣬Ȼ����� parse_ �ı�־�ж��Ƿ�������� DFA��
	// Ȼ����� AST ���ڵ��Ӧ�Ĺ����Զ��������������� dfaStart ��Ա��
	bool				Parse();
	// �ڽӿ��е��ô˷������˷��������ַ������������ĵ�һ��λ��, 
	// ���� match ��־�ж��Ƿ���ƥ�����������ƥ�䣬���Զ�����������һ���ĳ���
	// ����� isDFA_ ��ѡ����� rundfa ���� runnfa ��
	// �Ҳ����᷵�� -1
	std::pair<int, int>	Find(int begin, const std::string &matchContent, bool match); 
};

#endif 