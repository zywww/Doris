#include "Token.h"

Token::Token(TokenType type, char ch1, char ch2 = '\0') :
	type_(type), lexeme_{ ch1, ch2 }
{
}