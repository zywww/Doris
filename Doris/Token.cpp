#include "Token.h"

Token::Token(TokenType type, char ch) :
	type_(type), lexeme_(ch)
{
}