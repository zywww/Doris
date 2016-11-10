#ifndef DORIS_PARSER_H__
#define DORIS_PARSER_H__

#include "Lexer.h"

class Parser
{
public:
	Parser(const char *);

	ASTNode* Parse();		// ��������ʽ�����﷨���������������жϣ����ɹ��򷵻� AST �����
private:
	void Error();
	void GetNextToken();

	const char* regex_;
	Lexer		lexer_;
};

#endif 