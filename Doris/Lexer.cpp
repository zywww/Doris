#include <cassert>
#include <iostream>
#include <regex>
#include <vector>
#include <utility>
#include "Lexer.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;

enum class State
{
	START,
	BACKSLASH,
};

Lexer::Lexer(const string &regex) : regex_(regex)
{
	// ��Ϊһ��������ʽ��һ�� lexer ��Ӧ�����Խ������ڹ��캯����
	Analyze();
	canGenDFA_ = isDFA();
}

Token Lexer::GetNextToken()
{
	if (tokenIndex_ < stream_.size())
		return stream_[tokenIndex_++];
	else
		return Token(TokenType::END);
}

bool Lexer::GetIsDFA()
{
	return canGenDFA_;
}

void Lexer::Error(const string &info)
{
	cout << info << endl;
	throw info;
}

void Lexer::Analyze()
{
	State state = State::START;
	string::size_type index = 0;
	// ���ʷ���Ԫ��� stream_
	auto push = [&](TokenType type, char ch = '\0') -> void
	{
		stream_.push_back(Token(type, ch ));	
		++index;
		state = State::START;
	};
	// ������λʮ������ֵ�������Ϸ������� -1
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
			case '-': push(TokenType::MINUS); break;
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
			case 'k': push(TokenType::NAMEREF); break;
			case 'x': 
				if (index + 3 > regex_.size())
					Error("\\x ����Ӧƥ����λʮ�������ַ�");
				hexval = getHexVal(regex_[index + 1], regex_[index + 2]);
				if (hexval >= 0 && hexval <= 127)
				{
					push(TokenType::SIMPLECHAR, hexval);
					index += 2;
				}
				else
					Error("\\x �����ʮ�������ַ����Ϸ�");
				break;
			default: 
				if (ch >= '1' && ch <= '9')
					push(TokenType::BACKREF, ch);
				else
					push(TokenType::SIMPLECHAR, ch);
				break;
			}
		}
	}
	if (state == State::BACKSLASH)
		Error("�յ�ת���ַ�");
}

bool Lexer::isDFA()
{
	for (decltype(stream_.size()) i = 0; i < stream_.size(); ++i)
	{
		auto token = stream_[i];
		switch (token.type_)
		{
		case TokenType::BACKREF:	// ���� \number �Ĳ��Ǵ�����
		case TokenType::NAMEREF:	// ���� \k �Ĳ��Ǵ�����
		case TokenType::BOUND:		// ���� \b
		case TokenType::NOT_BOUND:	// ���� \B
		case TokenType::DOLLAR:		// ���� $
			return false;
			break;
		case TokenType::NEGATE:		// ���� ^ �� ���� [^] ��ʽ��
			if (i == 0 || stream_[i - 1].type_ != TokenType::LBRACKET)
				return false;
			break;
			// ����� break û�����ã���������Ժ������ return ��䲻Ҫ�ˣ���� break 
			// �Ϳ�����ȷ��������⣬˵������һ������
		case TokenType::RBRACE:		// �� } * + ? ������� ? ��ʾ��̰����
		case TokenType::STAR:
		case TokenType::PLUS:
		case TokenType::QUERY:
			if (i + 1 < stream_.size() && stream_[i + 1].type_ == TokenType::QUERY)
				return false;
			break;
		case TokenType::LP:			// ���� ( ���Ҳ��� (?: �Ķ����Ǵ����򣬶����ڷ��鹹��
			if (i + 1 < stream_.size() && i + 2 < stream_.size() &&
				(stream_[i + 1].type_ != TokenType::QUERY ||
				stream_[i + 2].lexeme_ != ':'))
				return false;
			break;
		}
	}
	return true;

}

char* Lexer::DivideCharSet()
{
	// DFA ����Ҫ��NFA �й��ܱ�
	char* table = new char[128];
	return table;
	/*
	vector<pair<char, char>> pvec;
	int index = 0;
	while (index < stream_.size())
	{
		auto token = stream_[index];
		switch (token.type_)
		{
		case TokenType::SIMPLECHAR: 
			pvec.push_back(std::make_pair(token.lexeme_, token.lexeme_)); 
			break;
		case TokenType::LBRACKET: 
			++index;
			while (index < stream_.size() && stream_[index].type_ != TokenType::RBRACKET)
			{
				switch (stream_[index].type_)
				{
				case TokenType::WORD: 
					pvec.push_back(std::make_pair())
				}
			}
			break;
		}

	}
	*/
}