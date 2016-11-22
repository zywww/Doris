#ifndef DORIS_PARSER_H__
#define DORIS_PARSER_H__

#include <tuple>	
#include <string>
#include "Lexer.h"
#include "AST.h"

class Parser
{
public:
	Parser(const std::string &regex);

	// ��������ʽ�����﷨���������ɹ��򷵻� AST �����
	ASTNode*					Parse();			
	bool						isDFA();

private:
	// �﷨����
	void						Error(const std::string &info);			
	// ͨ�� lexer_ ��ȡ�¸��ʷ���Ԫ
	void						GetNextToken();		
	
	// ��鵱ǰ�ʷ���Ԫ�Ƿ��� ���ַ� ch
	bool						Match(char ch);
	// ��鵱ǰ�ʷ���Ԫ�Ƿ��� ĳ����Ԫ�ַ�
	bool						Match(TokenType type);

	// �ݹ��½����﷨����
	ASTNode*					Regex();
	ASTNode*					Term();
	ASTNode*					Factor();
	std::pair<ASTNode*, bool>	Atom();
	std::tuple<bool, int, int>	Repeat();			// �����ظ��Ĵ��� min, max �� �Ƿ�̰���ظ�
	ASTNode*					Charclass();
	ASTNode*					UnnameCapture();
	ASTNode*					NameCapture();
	ASTNode*					NotCapture();
	ASTNode*					PositiveLookahead();
	ASTNode*					NegativeLookahead();
	int							Number();
	std::string					Name();
	

	const std::string&	regex_;
	Lexer				lexer_;
	Token				token_;
	ASTNode*			root_ = nullptr;
	int					count_ = 1;
};

#endif 