#include <string>
#include "Regex.h"

using std::string;

Regex::Regex(const string &regex) :
	regex_(regex), parser_(regex_), automaton_(parser_.GetASTRoot())
{
}

bool Regex::Match(const string& matchContent)
{
	return true;
}