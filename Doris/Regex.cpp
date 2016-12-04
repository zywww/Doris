#include <string>
#include <iostream>
#include "Regex.h"

using std::string;

Regex::Regex(const string &regex) :
	regex_(regex), parser_(regex_), automaton_(parser_.GetASTRoot())
{
}

bool Regex::Match(const string& matchContent)
{
	//std::cout << "parser_.notgreedy:" << parser_.GetNotGreedy() << std::endl;
	auto pair = automaton_.RunNFA(matchContent, 0, true);
	std::cout << pair.first << " " << pair.second << std::endl;
	if (pair.second == matchContent.size())	return true;
	else return false;
}

bool Regex::Search(const std::string& searchContent, int startIndex)
{
	auto pair = automaton_.RunNFA(searchContent, startIndex, false);
	return true;
}