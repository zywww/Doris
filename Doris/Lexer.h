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
	Token							Lookahead();
	Token							Backoff();
	// ��������ʽ�Ƿ�������� DFA
	bool							GetIsDFA();
	

private:
	// �ʷ����󣬴�ӡ������Ϣ��ֱ�� assert false
	void							Error(const std::string &info);
	// ��Ϊ������ʽһ�㲻��ܳ������Կ���һ���Խ����ɴʷ���Ԫ��
	void							Analyze();
	// ����һ��ʷ���Ԫ���ж��Ƿ�����չ�ĳɷ֣���û����������� DFA
	bool							isDFA();
	// ����һ��ʷ���Ԫ��ȷ���ַ���
	char*							DivideCharSet();	// �� regex ������������л��ոÿռ�
	// �ʷ��������� TODO �ж��Ƿ����
	int								Number(std::string::size_type index);			
		
	const std::string&				regex_;
	std::vector<Token>				stream_;
	std::vector<Token>::size_type	tokenIndex_ = 0;
	bool							canGenDFA_;
};

#endif
