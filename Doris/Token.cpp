#include "Token.h"

Token::Token(TokenType type, char ch) :
	type_(type), lexeme_(ch)
{
}

bool operator==(const Token &lhs, const Token &rhs)
{
	return lhs.type_ == rhs.type_ && lhs.lexeme_ == rhs.lexeme_;
}
