#ifndef DORIS_LEXER_H__
#define DORIS_LEXER_H__

#include <string>
#include <vector>
#include "Token.h"

class Lexer
{
public:
	Lexer(const std::string &regex);

	// 如果 tokenStream 到达末尾，返回 END
	Token							GetNextToken();	

private:
	// 词法错误，打印错误信息，直接 assert false
	void							Error(const std::string &info);

	// 因为正则表达式一般不会很长，所以可以一次性解析成词法单元流
	void							Analyze();
		
	const std::string&				regex_;
	std::vector<Token>				steam_;
	std::vector<Token>::size_type	tokenIndex_ = 0;
};

#endif
