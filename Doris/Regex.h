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
	Regex(const std::string &regex);
	

	bool				Match(const std::string& matchContent);
	std::pair<int, int>	Search(const std::string& searchContent, int startIndex = 0);	// ��ʱ����
	bool				Replace();	// ��ʱ����


private:
	const std::string	regex_;
	Parser				parser_;
	Automaton			automaton_;
};

#endif 