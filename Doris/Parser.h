#ifndef DORIS_PARSER_H__
#define DORIS_PARSER_H__

#include <tuple>	
#include <string>
#include <unordered_set>	
#include <vector>
#include "Lexer.h"
#include "AST.h"

class Parser
{
public:
	Parser(const std::string &regex);
	~Parser();
			
	ASTNode*							GetASTRoot();

private:
	// �﷨���󣬴�ӡ������Ϣ�����׳��쳣
	void								Error(const std::string &info);			
	// ��ȡ�¸��ʷ���Ԫ
	void								GetNextToken();	
	// ����һ���ʷ���Ԫ
	void								Backoff();
	// ��鵱ǰ�ʷ���Ԫ�Ƿ��� ���ַ� ch
	bool								Match(char ch);
	// ��鵱ǰ�ʷ���Ԫ�Ƿ��� ĳ����Ԫ�ַ�
	bool								Match(TokenType type);

	// ��������ʽ�����﷨���������ɹ��򷵻� AST �����
	ASTNode*							Parse();

	// �ݹ��½����﷨����
	ASTNode*							Regex();
	ASTNode*							Term();
	ASTNode*							Factor();
	// ���ؽ����Ƿ��ǿ����ظ��Ľṹ
	std::pair<ASTNode*, bool>			Atom();
	// �����ظ��Ĵ��� min, max �� �Ƿ�̰���ظ�
	std::tuple<bool, int, int>			Repeat();			
	ASTNode*							Charclass();
	ASTNode*							UnnameCapture();
	ASTNode*							NameCapture();
	ASTNode*							NotCapture();
	ASTNode*							PositiveLookahead();
	ASTNode*							NegativeLookahead();
	int									Number();
	std::string							Name();
	bool								IsAtomBegin(Token token);

	Lexer								lexer_;
	Token								token_;
	ASTNode*							astRoot_ = nullptr;
	int									count_ = 1;
	std::unordered_set<std::string>		referenceSet_;
};

#endif 