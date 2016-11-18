#ifndef DORIS_PARSER_H__
#define DORIS_PARSER_H__

#include <string>
#include "Lexer.h"
#include "AST.h"

class Parser
{
public:
	Parser(const std::string &regex);

	ASTNode*			Parse();		// ��������ʽ�����﷨���������������жϣ����ɹ��򷵻� AST �����
	bool				isDFA();

private:
	void				Error();			// �﷨����
	void				GetNextToken();	// ͨ�� lexer_ ��ȡ�¸��ʷ���Ԫ

	const std::string&	regex_;
	Lexer				lexer_;
};

#endif 