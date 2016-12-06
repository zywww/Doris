#ifndef DORIS_LEXER_H__
#define DORIS_LEXER_H__

#include <string>
#include <vector>
#include "Token.h"

class Lexer
{
public:
	Lexer(const std::string &regex);

	// ������һ���ʷ���Ԫ����� tokenStream ����ĩβ������ END �ʷ���Ԫ
	Token							GetNextToken();	
	// ����һ���ʷ���Ԫ
	Token							Backoff();

private:
	// �ʷ����󣬴�ӡ������Ϣ���׳��쳣
	void							Error(const std::string &info);
	// ��������ʽһ���Է����ɴʷ���Ԫ��������� stream_ ��
	void							Analyze();

	const std::string&				regex_;
	std::vector<Token>				stream_;
	std::vector<Token>::size_type	tokenIndex_ = 0;
};

#endif
