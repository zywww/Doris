#ifndef DORIS_PARSER_H__
#define DORIS_PARSER_H__

#include <string>
#include "Lexer.h"
#include "AST.h"

class Parser
{
public:
	Parser(const std::string &regex);

	ASTNode*			Parse();		// 对正则表达式进行语法分析，若错误则中断，若成功则返回 AST 根结点
	bool				isDFA();

private:
	void				Error();			// 语法错误
	void				GetNextToken();	// 通过 lexer_ 获取下个词法单元

	const std::string&	regex_;
	Lexer				lexer_;
};

#endif 