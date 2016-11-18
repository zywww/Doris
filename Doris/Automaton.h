#ifndef DORIS_AUTOMATON_H__
#define DORIS_AUTOMATON_H__

#include <unordered_map>

class Automaton
{
public:
	// name begin, end
	std::unordered_map<std::string, std::pair<size_t, size_t>> captureContent_;

};


#endif