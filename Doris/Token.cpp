#include "Token.h"

Token::Token(TokenType type, char ch1, char ch2) :
	type_(type), lexeme_{ ch1, ch2 }
{
}