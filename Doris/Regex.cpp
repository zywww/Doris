#include "Debug.h"
#include <string>
#include <iostream>
#include <utility>
#include "Regex.h"

using std::string;
using std::pair;

Regex::Regex(const string &regex) :
	regex_(regex), parser_(regex_), automaton_(parser_.GetASTRoot())
{
}

bool Regex::Match(const string& matchContent)
{
#ifdef DORIS_DEBUG
	clock_t start = clock(), end;
	start = clock();
#endif

	//std::cout << "parser_.notgreedy:" << parser_.GetNotGreedy() << std::endl;
	auto pair = automaton_.RunNFA(matchContent, 0, true);
	//std::cout << pair.first << " " << pair.second << std::endl;

#ifdef DORIS_DEBUG
	end = clock();
	std::cout << "Match: " << (double)(end - start) << "ms" << std::endl;
#endif

	if (pair.second == matchContent.size())	return true;
	else return false;



}

pair<int, int> Regex::Search(const std::string& searchContent, int startIndex)
{
	auto pair = automaton_.RunNFA(searchContent, startIndex, false);
	return pair;
}