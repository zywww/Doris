#ifndef DORIS_AUTOMATON_H__
#define DORIS_AUTOMATON_H__

#include <unordered_map>
#include <vector>




class Automaton
{
public:
	

	std::pair<size_t, size_t> GetCaptureContent(std::string name);
	void PushPair(int lhs, int rhs);

	// 匿名和命名捕获的内容都存放在这里
	// name begin, end
	std::unordered_map<std::string, std::pair<size_t, size_t>> captureContents_;



	const std::string&		regex = std::string();
	std::string::size_type	index;
	bool					singleLine_ = false;
};


#endif