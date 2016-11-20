#ifndef DORIS_PARSER_H__
#define DORIS_PARSER_H__

#include <string>
#include "Lexer.h"
#include "AST.h"

class Parser
{
public:
	Parser(const std::string &regex);

	ASTNode*					Parse();			// ��������ʽ�����﷨���������������жϣ����ɹ��򷵻� AST �����
	bool						isDFA();

private:
	void						Error(const std::string &info);			// �﷨����
	void						GetNextToken();		// ͨ�� lexer_ ��ȡ�¸��ʷ���Ԫ
	// �鿴��һ���ʷ���Ԫ�Ƿ�ƥ�䣬��ָ�벻ǰ��
	bool						Lookahead(Token token);
	// �鿴��һ���ʷ���Ԫ�Ƿ�ƥ��
	bool						Match(char ch);
	bool						Match(TokenType type);
	bool						MatchAndGoahead(char ch);
	bool						MatchAndGoahead(TokenType type);

	// �ݹ��½����﷨����
	ASTNode*					Regex();
	ASTNode*					Term();
	ASTNode*					Factor();
	std::pair<ASTNode*, bool>	Atom();
	std::tuple<bool, int, int>	Repeat();			// �����ظ��Ĵ��� min, max �� �Ƿ�̰���ظ�
	ASTNode*					Charclass();
	ASTNode*					GroupConstruct();
	ASTNode*					UnnameCapture();
	ASTNode*					NameCapture();
	ASTNode*					NotCapture();
	ASTNode*					PositiveLookahead();
	ASTNode*					NegativeLookahead();
	

	const std::string&	regex_;
	Lexer				lexer_;
	Token				token_;
};

#endif 