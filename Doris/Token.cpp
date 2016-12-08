#include "Debug.h"
#include "Token.h"


Token::Token(TokenType type, char ch) :
	type_(type), lexeme_(ch)
{
	switch (type_)
	{
	case TokenType::OR:			lexeme_ = '|'; break;
	case TokenType::STAR:		lexeme_ = '*'; break;
	case TokenType::PLUS:		lexeme_ = '+'; break;
	case TokenType::QUERY:		lexeme_ = '?'; break;
	case TokenType::NEGATE:		lexeme_ = '^'; break;
	case TokenType::DOLLAR:		lexeme_ = '$'; break;
	case TokenType::LBRACE:		lexeme_ = '{'; break;
	case TokenType::RBRACE:		lexeme_ = '}'; break;
	case TokenType::LBRACKET:	lexeme_ = '['; break;
	case TokenType::RBRACKET:	lexeme_ = ']'; break;
	case TokenType::LP:			lexeme_ = '('; break;
	case TokenType::RP:			lexeme_ = ')'; break;
	case TokenType::LANGLE:		lexeme_ = '<'; break;
	case TokenType::RANGLE:		lexeme_ = '>'; break;
	case TokenType::ANY:		lexeme_ = '.'; break;
	case TokenType::MINUS:		lexeme_ = '-'; break;
	}
}

bool operator==(const Token &lhs, const Token &rhs)
{
	return lhs.type_ == rhs.type_ && lhs.lexeme_ == rhs.lexeme_;
}
