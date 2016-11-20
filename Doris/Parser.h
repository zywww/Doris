#ifndef DORIS_PARSER_H__
#define DORIS_PARSER_H__

#include <string>
#include "Lexer.h"
#include "AST.h"

class Parser
{
public:
	Parser(const std::string &regex);

	ASTNode*					Parse();			// 对正则表达式进行语法分析，若错误则中断，若成功则返回 AST 根结点
	bool						isDFA();

private:
	void						Error(const std::string &info);			// 语法错误
	void						GetNextToken();		// 通过 lexer_ 获取下个词法单元
	// 查看下一个词法单元是否匹配，但指针不前进
	bool						Lookahead(Token token);
	// 查看下一个词法单元是否匹配
	bool						Match(char ch);
	bool						Match(TokenType type);
	bool						MatchAndGoahead(char ch);
	bool						MatchAndGoahead(TokenType type);

	// 递归下降的语法分析
	ASTNode*					Regex();
	ASTNode*					Term();
	ASTNode*					Factor();
	std::pair<ASTNode*, bool>	Atom();
	std::tuple<bool, int, int>	Repeat();			// 返回重复的次数 min, max 和 是否贪婪重复
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