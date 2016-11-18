#include <iostream>
#include "Parser.h"

using std::cout;
using std::endl;
using std::string;

Parser::Parser(const string &regex) : 
	regex_(regex), lexer_(regex), token_(TokenType::END)
{
}

ASTNode* Parser::Parse()
{
	return Regex();
}

ASTNode* Parser::Regex()
{
	ASTOR* node = new ASTOR;

}

bool Parser::isDFA() { return lexer_.GetIsDFA(); }

void Parser::Error(const string &info)
{
	cout << info << endl;
	throw info;
}



