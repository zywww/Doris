#ifndef DORIS_PARSER_H__
#define DORIS_PARSER_H__

#include "Lexer.h"

class Parser
{
public:
	Parser(const char *);

	ASTNode* Parse();		// 对正则表达式进行语法分析，若错误则中断，若成功则返回 AST 根结点
private:
	void Error();
	void GetNextToken();

	const char* regex_;
	Lexer		lexer_;
};

#endif 