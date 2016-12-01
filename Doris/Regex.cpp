#include <string>
#include "Regex.h"

using std::string;

Regex::Regex(const string &regex) :
	regex_(regex), parser_(regex_), automaton_(parser_.GetASTRoot())
{
}

bool Regex::Match(const string& matchContent)
{
	auto pair = automaton_.RunNFA(matchContent);
	if (pair.second == matchContent.size())	return true;
	else return false;
}

bool Regex::Search(const std::string& searchContent, int startIndex)
{
	return true;
}