#include <string>
#include "Regex.h"

using std::string;

Regex::Regex(const string &regex) :
	regex_(regex), parser_(regex_)
{
	auto root = parser_.GetRoot();
	auto pair = root->ConstructNFA();
}

bool Regex::Parse()
{
	/*
	auto root = parser_.Parse();
	isDFA_ = parser_.isDFA();
	if (isDFA_)
	{
	//	DFAStart_ = root->constructDFA();
		
	}
	else
	{
	//	root->constructNFA();
	}
	*/
	return true;

}