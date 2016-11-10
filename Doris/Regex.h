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

	// ���÷���ǰ�����ж��Ƿ� Parse����δ Parse������� Parse ����
	// Parse ���������﷨�������� Parse���õ� AST ����㣬
	// Ȼ������﷨�������ı�־�ж������� DFA ���� NFA��Ȼ���ö�Ӧ��
	// �Զ�����㣬�ӿڷ���ͨ�������Զ�����ɶ�Ӧ����
	bool				Match(const std::string &matchContent);
	bool				Search();
	bool				Replace();

private:
	const std::string	regex_;
	bool				parse_;
	NFAState*			NFAStart;
	DFAState*			DFAStart;

	bool				Parse();
};

#endif 