#ifndef DORIS_PARSER_H__
#define DORIS_PARSER_H__

#include <tuple>	
#include <string>
#include <unordered_set>	
#include "Lexer.h"
#include "AST.h"

class Parser
{
public:
	Parser(const std::string &regex);

	// ��������ʽ�����﷨���������ɹ��򷵻� AST �����
	ASTNode*							Parse();			
	bool								isDFA();

private:
	// �﷨����
	void								Error(const std::string &info);			
	// ͨ�� lexer_ ��ȡ�¸��ʷ���Ԫ
	void								GetNextToken();		
	void								Backoff();
	
	// ��鵱ǰ�ʷ���Ԫ�Ƿ��� ���ַ� ch
	bool								Match(char ch);
	// ��鵱ǰ�ʷ���Ԫ�Ƿ��� ĳ����Ԫ�ַ�
	bool								Match(TokenType type);

	// �ݹ��½����﷨����
	ASTNode*							Regex();
	ASTNode*							Term();
	ASTNode*							Factor();
	std::pair<ASTNode*, bool>			Atom();
	std::tuple<bool, int, int>			Repeat();			// �����ظ��Ĵ��� min, max �� �Ƿ�̰���ظ�
	// charclass Ӧ�÷ֳ���һ�������жϷ�Χ
	ASTNode*							Charclass();
	ASTNode*							UnnameCapture();
	ASTNode*							NameCapture();
	ASTNode*							NotCapture();
	ASTNode*							PositiveLookahead();
	ASTNode*							NegativeLookahead();
	int									Number();
	std::string							Name();
	
	bool								IsAtomBegin(Token token);

	const std::string&					regex_;
	Lexer								lexer_;
	Token								token_;
	ASTNode*							root_ = nullptr;
	int									count_ = 1;
	std::unordered_set<std::string>		nameReferenceSet_;
};

#endif 