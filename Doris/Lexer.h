#ifndef DORIS_LEXER_H__
#define DORIS_LEXER_H__

#include <string>
#include <vector>
#include "Token.h"

class Lexer
{
public:
	// 因为一个正则表达式和一个 lexer 对应，所以解析放在构造函数中
	Lexer(const std::string &regex);

	// 如果 tokenStream 到达末尾，返回 END
	Token							GetNextToken();	

private:
	// 词法错误，则 assert
	void							Error(const std::string &info);

	// 因为正则表达式一般不会很长，所以可以一次性解析成词法单元流
	void							Analyze();
		
	const std::string&				regex_;
	std::vector<Token>				steam_;
	std::vector<Token>::size_type	tokenIndex_ = 0;
};

#endif
