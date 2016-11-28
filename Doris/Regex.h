#ifndef DORIS_REGEX_H__
#define DORIS_REGEX_H__

#include <string>
#include "NFA.h"
#include "DFA.h"
#include "Parser.h"
#include "Automaton.h"

// Ҫ���ٵ���Դ�� AST �� �Զ��� ����Դ
class Regex
{
public:
	// �����ڹ��캯���б���������ʽ����Ϊһ�� regex ���ʾһ��������ʽ
	Regex(const std::string &regex, const std::string &matchContent);

	// match �ȵ��� parse �ٵ��� find ��õ�һ����������λ�ã�
	bool				Match(const std::string &matchContent);
	bool				Search();	// ��ʱ����
	bool				Replace();	// ��ʱ����

private:
	const std::string	regex_;
	const std::string   matchContent_;
	Parser				parser_;
	Automaton			automaton_;
	bool				isDFA_;


	bool				Parse();
	std::pair<int, int>	Find(int begin, const std::string &matchContent, bool match); 
};

#endif 