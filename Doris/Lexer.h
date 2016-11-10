#ifndef DORIS_LEXER_H__
#define DORIS_LEXER_H__

#include "Token.h"

class Lexer
{
public:
	Lexer(const char *regex);

	Token			GetNextToken();

private:
	void			Error(); // ´Ê·¨´íÎó£¬assert

	const char*		regex_;
};

#endif
