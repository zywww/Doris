#include <iostream>
#include <tuple>	
#include <cctype>	
#include "Parser.h"

using std::cout;
using std::endl;
using std::string;
using std::get;
using std::pair;

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

bool Parser::Match(char ch)
{
	return token_.type_ == TokenType::SIMPLECHAR && token_.lexeme_ == ch;
}

bool Parser::Match(TokenType type)
{
	return token_.type_ == type;
}

ASTNode* Parser::Regex()
{
	ASTOR* root = new ASTOR;
	if (Match(TokenType::END))
		return root;
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

pair<ASTNode*, bool> Parser::Atom()
{
	ASTNode* node = nullptr;
	bool repeat = true;
	switch (token_.type_)
	{
	case TokenType::SIMPLECHAR:
		node = new ASTCharacter(token_.lexeme_);
		GetNextToken();
		break;
		
	case TokenType::NEGATE: 
		node = new ASTAnchor(AnchorType::BEGIN);
		repeat = false;
		GetNextToken();
		break;

	case TokenType::DOLLAR:
		node = new ASTAnchor(AnchorType::END);
		repeat = false;
		GetNextToken();
		break;

	case TokenType::BOUND:
		node = new ASTAnchor(AnchorType::BOUND);
		repeat = false;
		GetNextToken();
		break;

	case TokenType::NOT_BOUND:
		node = new ASTAnchor(AnchorType::NOT_BOUND);
		repeat = false;
		GetNextToken();
		break;

	case TokenType::BACKREF:
		node = new ASTBackReference(token_.lexeme_ - '0');
		GetNextToken();
		break;

	case TokenType::NAMEREF:
		if (Match(TokenType::LANGLE))
		{
			string name;
			GetNextToken();
			name = Name();
			// Name() ���� > �ŷ���������˲���Ҫ���
			GetNextToken();
			node = new ASTNameReference(name);
		}
		else
			Error("�������óɷ�ȱ��");
		break;

	case TokenType::LBRACKET:
		GetNextToken();
		node = Charclass();
		break;

	case TokenType::LP:
		GetNextToken();
		if (Match(TokenType::QUERY))
		{
			GetNextToken();
			if (Match(':'))
				node = NotCapture();
			else if (Match(TokenType::LANGLE))
				node = NameCapture();
			else if (Match('='))
			{
				node = PositiveLookahead();
				repeat = false;
			}
			else if (Match('!'))
			{
				node = NegativeLookahead();
				repeat = false;
			}
			else
				Error("(? �Ƿ�");
		}
		else
			node = UnnameCapture();
	}
	return std::make_pair(node, repeat);
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
		if (token_.lexeme_ > '0' && token_.lexeme_ < '9')
			min = Number();
		else
			Error("{ } ��ȱ������");
		if (Match(','))
		{
			GetNextToken();
			if (Match(TokenType::RBRACE))
				max = -1;
			else if (token_.lexeme_ > '0' && token_.lexeme_ < '9')
				max = Number();
			else
				Error(" , �ɷִ���");
		}
		else if (Match(TokenType::RBRACE))
			max = -1;
		else
			Error("{ } �ɷִ���");
		if (Match(TokenType::QUERY))
		{
			greedy = true;
			GetNextToken();
		}
	}
	return	std::make_tuple(greedy, min, max);
}

ASTNode* Parser::Charclass()
{
	ASTCharClass* node;
	if (Match(TokenType::NEGATE))
	{
		node = new ASTCharClass(true);
		GetNextToken();
	}
	else
		node = new ASTCharClass(false);
	while (!Match(TokenType::RBRACKET))
	{
		if (Match(TokenType::END)) Error("[ ] �������");
		char lhs = token_.lexeme_;
		GetNextToken();
		if (Match(TokenType::MINUS))
		{
			GetNextToken();
			if (!Match(TokenType::SIMPLECHAR) || token_.lexeme_ < lhs)
				Error("[ ] �������");
			else
			{
				node->Push(std::make_pair(lhs, token_.lexeme_));
				GetNextToken();
			}
		}
		else
			node->Push(std::make_pair(lhs, lhs));
	}
	return node;
}

ASTNode* Parser::UnnameCapture()
{
	ASTNode* node = new ASTUnnameCapture(Regex(), count_++);
	if (!Match(TokenType::RP))
		Error("ȱ�� )");
	GetNextToken();
	return node;
}

// ����ú���ʱ��ȷ���� (?:
ASTNode* Parser::NotCapture()
{
	GetNextToken();
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("ȱ�� )");
	GetNextToken();
	return node;
}

// ����ú�����ȷ���� (?<
ASTNode* Parser::NameCapture()
{
	GetNextToken();
	std::string name = Name(); // Name ���� > ���˳�
	GetNextToken();
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("ȱ�� )");
	GetNextToken();
	return new ASTNameCapture(node, name);
}

// ����ú�����ȷ�� (?= ��ͷ
ASTNode* Parser::PositiveLookahead()
{
	GetNextToken();
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("ȱ�� )");
	GetNextToken();
	return new ASTPstLookahead(node);
}

// ����ú�����ȷ�� (?! ��ͷ
ASTNode* Parser::NegativeLookahead()
{
	GetNextToken();
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("ȱ�� )");
	GetNextToken();
	return new ASTNgtLookahead(node);
}

// ���� Number ʱ��ȷ���Ѿ���һλ����
int Parser::Number()
{
	long long ans = 0;
	while (token_.lexeme_ > '0' && token_.lexeme_ < '9')
	{
		ans = ans * 10 + (token_.lexeme_ - '0');
		if (ans > INT_MAX)
			Error("�������");
		GetNextToken();
	}
	return ans;
}

string Parser::Name()
{
	string ans;
	if (std::isdigit(token_.lexeme_))
		Error("���Ʋ��������ֿ�ͷ");
	while (!Match(TokenType::RANGLE))
	{
		if (!Match(TokenType::SIMPLECHAR))
			Error("<> �ڽṹ����");
		else if (std::isalnum(token_.lexeme_) || token_.lexeme_ == '_')
			ans += token_.lexeme_;
		else
			Error("<> �ڵ�����Ӧ��Ϊ�Ϸ��ı�ʶ��");
		GetNextToken();
	}
	if (ans.empty())
		Error("��������Ϊ��");
	return ans;
}

