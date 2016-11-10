#ifndef DORIS_LEXER_H__
#define DORIS_LEXER_H__

#include "Token.h"

class Lexer
{
public:
	Lexer(const char *regex);

	Token		GetNextToken();

private:
	void			Error();

	const char*		regex_;
};

#endif
