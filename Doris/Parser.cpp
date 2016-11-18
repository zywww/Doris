#include "Parser.h"

using std::string;

Parser::Parser(const string &regex) : regex_(regex), lexer_(regex)
{
}

bool Parser::isDFA() { return lexer_.GetIsDFA(); }

