#include <cassert>
#include <iostream>
#include "Lexer.h"

using std::cout;
using std::endl;
using std::string;

enum class State
{
	START,
	BACKSLASH,

};

Lexer::Lexer(const string &regex) : regex_(regex)
{
	// cout << regex_ << endl;
	Analyze();
}

void Lexer::Analyze()
{
	State state = State::START;
	string::size_type index = 0;
	auto push = [&](TokenType type, char ch = '\0') -> void
	{
		steam_.push_back(Token(type, ch));	
		++index;
		state = State::START;
	};
	auto getHexVal = [](char ch1, char ch2) -> char
	{
		int ans = 0;
		if (ch1 >= '0' && ch1 <= '9') ans += ch1 - '0';
		else if (ch1 >= 'a' && ch1 <= 'f') ans += ch1 - 'a' + 10;
		else if (ch1 >= 'A' && ch1 <= 'F') ans += ch1 - 'A' + 10;
		else return -1;
		ans *= 16;
		if (ch2 >= '0' && ch2 <= '9') ans += ch2 - '0';
		else if (ch2 >= 'a' && ch2 <= 'f') ans += ch2 - 'a' + 10;
		else if (ch2 >= 'A' && ch2 <= 'F') ans += ch2 - 'A' + 10;
		else return -1;

		return ans;
	};
	while (index < regex_.size())
	{
		char ch = regex_[index];
		int hexval;
		switch (state)
		{
		case State::START:
			switch (ch)
			{
			case '|': push(TokenType::OR); break;
			case '*': push(TokenType::STAR); break;
			case '+': push(TokenType::PLUS); break; 
			case '?': push(TokenType::QUERY); break;
			case '^': push(TokenType::NEGATE); break;
			case '$': push(TokenType::DOLLAR); break;
			case '{': push(TokenType::LBRACE); break;
			case '}': push(TokenType::RBRACE); break;
			case '[': push(TokenType::LBRACKET); break;
			case ']': push(TokenType::RBRACKET); break;
			case '(': push(TokenType::LP); break;
			case ')': push(TokenType::RP); break;
			case '\\': state = State::BACKSLASH; ++index; break;
			default: push(TokenType::SIMPLECHAR, ch); break;
			}
			break;
		case State::BACKSLASH:
			switch (ch)
			{
			case 'n': push(TokenType::SIMPLECHAR, '\n'); break;
			case 'r': push(TokenType::SIMPLECHAR, '\r'); break;
			case 't': push(TokenType::SIMPLECHAR, '\t'); break;
			case 'v': push(TokenType::SIMPLECHAR, '\v'); break;
			case 'w': push(TokenType::WORD); break;
			case 'W': push(TokenType::NOT_WORD); break;
			case 's': push(TokenType::SPACE); break;
			case 'S': push(TokenType::NOT_SPACE); break;
			case 'd': push(TokenType::DIGIT); break;
			case 'D': push(TokenType::NOT_DIGIT); break;
			case 'b': push(TokenType::BOUND); break;
			case 'B': push(TokenType::NOT_BOUND); break;
			case 'x': 
				if (index + 3 > regex_.size())
					Error("\\x 后面应匹配两位十六进制字符");
				hexval = getHexVal(regex_[index + 1], regex_[index + 2]);
				if (hexval >= 0 && hexval <= 127)
				{
					push(TokenType::SIMPLECHAR, hexval);
					index += 2;
				}
				else
					Error("\\x 后面的十六进制字符不合法");
				break;
			default: push(TokenType::SIMPLECHAR, ch); break;
			}
		}
	}
	if (state == State::BACKSLASH)
		Error("空的转义字符");
}

Token Lexer::GetNextToken()
{
	if (tokenIndex_ < steam_.size())
		return steam_[tokenIndex_++];
	else
		return Token(TokenType::END);
}

void Lexer::Error(const string &info)
{
	cout << info << endl;
	assert(false);
}