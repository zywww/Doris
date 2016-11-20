#include <iostream>
#include "Parser.h"

using std::cout;
using std::endl;
using std::string;

Parser::Parser(const string &regex) : 
	regex_(regex), lexer_(regex), token_(TokenType::END)
{
	GetNextToken();
}

ASTNode* Parser::Parse()
{
	return Regex();
}

bool Parser::isDFA() { return lexer_.GetIsDFA(); }

void Parser::Error(const string &info)
{
	cout << info << endl;
	throw info;
}

void Parser::GetNextToken()
{
	token_ = lexer_.GetNextToken();
}

bool Parser::Lookahead(Token token)
{
	auto temp = lexer_.Lookahead();
	return token == temp;
}

bool Parser::Match(TokenType type)
{
	return token_.type_ == type;
}

ASTNode* Parser::Regex()
{
	if (Match(TokenType::END))
		return nullptr;

	ASTOR* node = new ASTOR;
	
}



