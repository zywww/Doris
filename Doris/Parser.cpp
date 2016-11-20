#include <iostream>
#include "Parser.h"

using std::cout;
using std::endl;
using std::string;
using std::get;

Parser::Parser(const string &regex) : 
	regex_(regex), lexer_(regex), token_(TokenType::END)
{
	GetNextToken();
}

ASTNode* Parser::Parse()
{
	return Regex();
}

bool Parser::isDFA() { return lexer_.GetIsDFA(); }

void Parser::Error(const string &info)
{
	cout << info << endl;
	throw info;
}

void Parser::GetNextToken()
{
	token_ = lexer_.GetNextToken();
}

bool Parser::Lookahead(Token token)
{
	auto temp = lexer_.Lookahead();
	return token == temp;
}

bool Parser::Match(TokenType type)
{
	return token_.type_ == type;
}

ASTNode* Parser::Regex()
{
	if (Match(TokenType::END))
		return nullptr;

	ASTOR* root = new ASTOR;
	root->Push(Term());
	while (Match(TokenType::OR))
	{
		GetNextToken();
		root->Push(Term());
	}
	
	if (Match(TokenType::END))
		return root;
	else
	{
		Error(" | ȱ������");
		return nullptr;
	}
}

ASTNode* Parser::Term()
{
	if (Match(TokenType::OR))
	{
		Error(" | ȱ������");
		return nullptr;
	}

	ASTCat* node = new ASTCat;
	node->Push(Factor());
	while (!Match(TokenType::OR))
		node->Push(Factor());
	return node;
}

// �� Term ���� Factor ʱ���Ѿ���֤ Factor ��Ϊ��
ASTNode* Parser::Factor()
{
	auto atom = Atom();
	switch (token_.type_)
	{
	case TokenType::STAR:
	case TokenType::PLUS:
	case TokenType::QUERY:
	case TokenType::LBRACE:
		if (atom.second) // ��Ϊ true �� atom �ǿ��ظ��Ĺ���
		{
			auto repeat = Repeat();
			return new ASTRepeat(atom.first, get<0>(repeat), get<1>(repeat), get<2>(repeat));
		}
		else
		{
			Error("�ù��첻���ظ�");
			return nullptr;
		}
		break;
	default:
		return atom.first;
		break;
	}
}

// �ܽ��� Repeat �ķ�����˵���ù����� * + ? { �ȷ��ſ�ʼ��
std::tuple<bool, int, int>	Parser::Repeat()
{
	bool greedy = true;
	int min, max;
	switch (token_.type_)
	{
	case TokenType::STAR:
		min = 0;
		max = -1;
		GetNextToken();
		if (Match(TokenType::QUERY))
		{
			greedy = false;
			GetNextToken();
		}
		break;

	case TokenType::PLUS:
		min = 1;
		max = -1;
		GetNextToken();
		if (Match(TokenType::QUERY))
		{
			greedy = false;
			GetNextToken();
		}
		break;

	case TokenType::QUERY:
		min = 0;
		max = 1;
		if (Match(TokenType::QUERY))
		{
			greedy = false;
			GetNextToken();
		}
		break;

	case TokenType::LBRACE:
		GetNextToken();

	}
}



