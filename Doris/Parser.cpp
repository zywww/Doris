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
		Error(" | 缺少因子");
		return nullptr;
	}
}

ASTNode* Parser::Term()
{
	if (Match(TokenType::OR))
	{
		Error(" | 缺少因子");
		return nullptr;
	}

	ASTCat* node = new ASTCat;
	node->Push(Factor());
	while (!Match(TokenType::OR))
		node->Push(Factor());
	return node;
}

// 在 Term 调用 Factor 时，已经保证 Factor 不为空
ASTNode* Parser::Factor()
{
	auto atom = Atom();
	switch (token_.type_)
	{
	case TokenType::STAR:
	case TokenType::PLUS:
	case TokenType::QUERY:
	case TokenType::LBRACE:
		if (atom.second) // 若为 true 则 atom 是可重复的构造
		{
			auto repeat = Repeat();
			return new ASTRepeat(atom.first, get<0>(repeat), get<1>(repeat), get<2>(repeat));
		}
		else
		{
			Error("该构造不可重复");
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
			// Name() 遇到 > 才返回来，因此不需要检查
			GetNextToken();
			node = new ASTNameReference(name);
		}
		else
			Error("命名引用成分缺少");
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
				Error("(? 非法");
		}
		else
			node = UnnameCapture();
	}
	return std::make_pair(node, repeat);
}

// 能进入 Repeat 的分析，说明该构造是 * + ? { 等符号开始的
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
			Error("{ } 内缺少数字");
		if (Match(','))
		{
			GetNextToken();
			if (Match(TokenType::RBRACE))
				max = -1;
			else if (token_.lexeme_ > '0' && token_.lexeme_ < '9')
				max = Number();
			else
				Error(" , 成分错误");
		}
		else if (Match(TokenType::RBRACE))
			max = -1;
		else
			Error("{ } 成分错误");
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
		if (Match(TokenType::END)) Error("[ ] 构造错误");
		char lhs = token_.lexeme_;
		GetNextToken();
		if (Match(TokenType::MINUS))
		{
			GetNextToken();
			if (!Match(TokenType::SIMPLECHAR) || token_.lexeme_ < lhs)
				Error("[ ] 构造错误");
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
		Error("缺少 )");
	GetNextToken();
	return node;
}

// 进入该函数时以确定有 (?:
ASTNode* Parser::NotCapture()
{
	GetNextToken();
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("缺少 )");
	GetNextToken();
	return node;
}

// 进入该函数以确定有 (?<
ASTNode* Parser::NameCapture()
{
	GetNextToken();
	std::string name = Name(); // Name 遇到 > 就退出
	GetNextToken();
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("缺少 )");
	GetNextToken();
	return new ASTNameCapture(node, name);
}

// 进入该函数以确定 (?= 开头
ASTNode* Parser::PositiveLookahead()
{
	GetNextToken();
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("缺少 )");
	GetNextToken();
	return new ASTPstLookahead(node);
}

// 进入该函数以确定 (?! 开头
ASTNode* Parser::NegativeLookahead()
{
	GetNextToken();
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("缺少 )");
	GetNextToken();
	return new ASTNgtLookahead(node);
}

// 调用 Number 时，确定已经有一位数字
int Parser::Number()
{
	long long ans = 0;
	while (token_.lexeme_ > '0' && token_.lexeme_ < '9')
	{
		ans = ans * 10 + (token_.lexeme_ - '0');
		if (ans > INT_MAX)
			Error("数字溢出");
		GetNextToken();
	}
	return ans;
}

string Parser::Name()
{
	string ans;
	if (std::isdigit(token_.lexeme_))
		Error("名称不能以数字开头");
	while (!Match(TokenType::RANGLE))
	{
		if (!Match(TokenType::SIMPLECHAR))
			Error("<> 内结构错误");
		else if (std::isalnum(token_.lexeme_) || token_.lexeme_ == '_')
			ans += token_.lexeme_;
		else
			Error("<> 内的名称应该为合法的标识符");
		GetNextToken();
	}
	if (ans.empty())
		Error("命名不能为空");
	return ans;
}

