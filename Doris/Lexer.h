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
	Token							Lookahead();
	Token							Backoff();
	// 该正则表达式是否可以生成 DFA
	bool							GetIsDFA();
	

private:
	// 词法错误，打印错误信息，直接 assert false
	void							Error(const std::string &info);
	// 因为正则表达式一般不会很长，所以可以一次性解析成词法单元流
	void							Analyze();
	// 遍历一遍词法单元，判断是否有扩展的成分，若没有则可以生成 DFA
	bool							isDFA();
	// 分析一遍词法单元，确定字符集
	char*							DivideCharSet();	// 在 regex 类的析构函数中回收该空间
	// 词法分析数字 TODO 判断是否溢出
	int								Number(std::string::size_type index);			
		
	const std::string&				regex_;
	std::vector<Token>				stream_;
	std::vector<Token>::size_type	tokenIndex_ = 0;
	bool							canGenDFA_;
};

#endif
