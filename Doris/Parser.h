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
	// 语法错误，打印错误信息，并抛出异常
	void								Error(const std::string &info);			
	// 获取下个词法单元
	void								GetNextToken();	
	// 回退一个词法单元
	void								Backoff();
	// 检查当前词法单元是否是 简单字符 ch
	bool								Match(char ch);
	// 检查当前词法单元是否是 某类型元字符
	bool								Match(TokenType type);

	// 对正则表达式进行语法分析，若成功则返回 AST 根结点
	ASTNode*							Parse();

	// 递归下降的语法分析
	ASTNode*							Regex();
	ASTNode*							Term();
	ASTNode*							Factor();
	// 返回结点和是否是可以重复的结构
	std::pair<ASTNode*, bool>			Atom();
	// 返回重复的次数 min, max 和 是否贪婪重复
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