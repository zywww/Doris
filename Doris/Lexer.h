#ifndef DORIS_LEXER_H__
#define DORIS_LEXER_H__

#include <string>
#include <vector>
#include "Token.h"

class Lexer
{
public:
	Lexer(const std::string &regex);

	// 返回下一个词法单元，如果 tokenStream 到达末尾，返回 END 词法单元
	Token							GetNextToken();	
	// 回退一个词法单元
	Token							Backoff();

private:
	// 词法错误，打印错误信息，抛出异常
	void							Error(const std::string &info);
	// 将正则表达式一次性分析成词法单元流，存放在 stream_ 内
	void							Analyze();

	const std::string&				regex_;
	std::vector<Token>				stream_;
	std::vector<Token>::size_type	tokenIndex_ = 0;
};

#endif
