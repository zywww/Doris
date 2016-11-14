#ifndef DORIS_LEXER_H__
#define DORIS_LEXER_H__

#include <string>
#include <vector>
#include "Token.h"

class Lexer
{
public:
	Lexer(const std::string &regex);

	// ��� tokenStream ����ĩβ������ END
	Token							GetNextToken();	

private:
	// �ʷ����󣬴�ӡ������Ϣ��ֱ�� assert false
	void							Error(const std::string &info);

	// ��Ϊ������ʽһ�㲻��ܳ������Կ���һ���Խ����ɴʷ���Ԫ��
	void							Analyze();
		
	const std::string&				regex_;
	std::vector<Token>				steam_;
	std::vector<Token>::size_type	tokenIndex_ = 0;
};

#endif
