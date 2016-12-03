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
	lexer_(regex), token_(TokenType::END)
{
	GetNextToken();
	astRoot_ = Parse();
}

ASTNode* Parser::Parse()
{
	if (Match(TokenType::END))
		return new ASTEmpty();
	ASTNode* root = Regex();
	if (!Match(TokenType::END))
		Error("���ʽ����");
	return root;
}
/*
bool Parser::isDFA() 
{
	return lexer_.GetIsDFA(); 
}
*/
ASTNode* Parser::GetASTRoot()
{
	return astRoot_;
}

bool Parser::GetNotGreedy()
{
	return notGreedy_;
}

void Parser::Error(const string &info)
{
	cout << info << endl;
	throw info;
}

void Parser::GetNextToken()
{
	token_ = lexer_.GetNextToken();
}

void Parser::Backoff()
{
	token_ = lexer_.Backoff();
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
	ASTOR* node = new ASTOR;
	if (Match(TokenType::OR))
		node->Push(new ASTEmpty());
	else 
		node->Push(Term());
	
	while (Match(TokenType::OR))
	{
		GetNextToken();
		if (Match(TokenType::OR))
			node->Push(new ASTEmpty());
		else if (IsAtomBegin(token_))
			node->Push(Term());
		else
			node->Push(new ASTEmpty());
	}
	
	return node;
}

ASTNode* Parser::Term()
{
	ASTCat* node = new ASTCat;
	node->Push(Factor());

	while (IsAtomBegin(token_))
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
	ASTCharClass* temp = nullptr;
	bool repeat = true;
	switch (token_.type_)
	{
	case TokenType::SIMPLECHAR:
		node = new ASTCharacter(token_.lexeme_);
		GetNextToken();
		break;

	case TokenType::ANY:
		temp = new ASTCharClass(true);
		temp->Push('\n', '\n');
		node = temp;
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
		// TODO �˴������ظ������Լ�
	case TokenType::WORD:
		temp = new ASTCharClass(false);
		temp->Push('a', 'z');
		temp->Push('A', 'Z');
		temp->Push('0', '9');
		temp->Push('_', '_');
		node = temp;
		GetNextToken();
		break;

	case TokenType::NOT_WORD:
		temp = new ASTCharClass(true);
		temp->Push('a', 'z');
		temp->Push('A', 'Z');
		temp->Push('0', '9');
		temp->Push('_', '_');
		node = temp;
		GetNextToken();
		break;

	case TokenType::SPACE:
		temp = new ASTCharClass(false);
		temp->Push(' ', ' ');
		temp->Push('\f', '\f');
		temp->Push('\n', '\n');
		temp->Push('\r', '\r');
		temp->Push('\t', '\t');
		temp->Push('\v', '\v');
		node = temp;
		GetNextToken();
		break;
	
	case TokenType::NOT_SPACE:
		temp = new ASTCharClass(true);
		temp->Push(' ', ' ');
		temp->Push('\f', '\f');
		temp->Push('\n', '\n');
		temp->Push('\r', '\r');
		temp->Push('\t', '\t');
		temp->Push('\v', '\v');
		node = temp;
		GetNextToken();
		break;

	case TokenType::DIGIT:
		temp = new ASTCharClass(false);
		temp->Push('0', '9');
		node = temp;
		GetNextToken();
		break;

	case TokenType::NOT_DIGIT:
		temp = new ASTCharClass(true);
		temp->Push('0', '9');
		node = temp;
		GetNextToken();
		break;

	case TokenType::BACKREF:
		if (token_.lexeme_ - '0' >= count_)
			Error("�������ô���");
		node = new ASTBackReference(token_.lexeme_ - '0');
		GetNextToken();
		break;

	case TokenType::NAMEREF:
		GetNextToken();
		if (Match(TokenType::LANGLE))
		{
			string name;
			GetNextToken();
			name = Name();
			// Name() ���� > �ŷ���������˲���Ҫ���
			GetNextToken();
			if (!nameReferenceSet_.count(name))
				Error("�������ҵ�����������");
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
			{
				GetNextToken();
				node = NotCapture();
			}
			else if (Match(TokenType::LANGLE))
			{
				GetNextToken();
				node = NameCapture();
			}
			else if (Match('='))
			{
				GetNextToken();
				node = PositiveLookahead();
				repeat = false;
			}
			else if (Match('!'))
			{
				GetNextToken();
				node = NegativeLookahead();
				repeat = false;
			}
			else
				Error("(? �Ƿ�");
		}
		else
			node = UnnameCapture();
		break;
	default: // �ĳɴ��󣿣�
		/*
		node = new ASTEmpty();
		repeat = true;
		*/
		Error("���Ӵ���");
		break;
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
		GetNextToken();
		if (Match(TokenType::QUERY))
		{
			greedy = false;
			GetNextToken();
		}
		break;

	case TokenType::LBRACE:
		GetNextToken();
		if (std::isdigit(token_.lexeme_))
			min = Number();
		else
			Error("{ } ��ȱ������");

		if (Match(','))
		{
			GetNextToken();
			if (Match(TokenType::RBRACE))
			{
				max = -1;
				GetNextToken();
			}
			else if (std::isdigit(token_.lexeme_))
			{
				max = Number();
				if (!Match(TokenType::RBRACE))
					Error("ȱ�� }");
				if (min > max)
					Error("{ } �� max ������ڵ��� min");
				GetNextToken();
			}
			else
				Error("{ } �ɷִ���");
		}
		else if (Match(TokenType::RBRACE))
		{
			max = min;
			GetNextToken();
		}
		else
			Error("{ } �ɷִ���");

		if (Match(TokenType::QUERY))
		{
			greedy = false;
			GetNextToken();
		}
	}

	notGreedy_ = !greedy;
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
	if (Match(TokenType::RBRACKET))
		Error("[ ] �ڲ�����Ϊ��");

	while (!Match(TokenType::RBRACKET))
	{
		char lhs;
		bool range = false;
		switch (token_.type_)
		{
		case TokenType::SIMPLECHAR:
		case TokenType::OR:
		case TokenType::STAR:
		case TokenType::PLUS:
		case TokenType::DOLLAR:
		case TokenType::QUERY:
		case TokenType::NEGATE:
		case TokenType::LBRACE:
		case TokenType::RBRACE:
		case TokenType::LP:
		case TokenType::RP:
		case TokenType::LANGLE:
		case TokenType::RANGLE:
		case TokenType::ANY:
		case TokenType::MINUS:
			lhs = token_.lexeme_;
			break;

		case TokenType::WORD:
			range = true;
			node->Push('a', 'z');
			node->Push('A', 'Z');
			node->Push('0', '9');
			node->Push('_', '_');
			GetNextToken();
			break;

		case TokenType::NOT_WORD:
			range = true;
			node->Push(0, '0' - 1);
			node->Push('9' + 1, 'A' - 1);
			node->Push('Z' + 1, 'a' - 1);
			node->Push('z' + 1, 127);
			GetNextToken();
			break;

		case TokenType::SPACE:
			range = true;
			node->Push(' ', ' ');
			node->Push('\f', '\f');
			node->Push('\n', '\n');
			node->Push('\r', '\r');
			node->Push('\t', '\t');
			node->Push('\v', '\v');
			GetNextToken();
			break;

		case TokenType::NOT_SPACE:
			range = true;
			node->Push('0', '\t' - 1);
			node->Push('\r' + 1, ' ' - 1);
			node->Push(' ' + 1, 127);
			GetNextToken();
			break;

		case TokenType::DIGIT:
			range = true;
			node->Push('0', '9');
			GetNextToken();
			break;

		case TokenType::NOT_DIGIT:
			range = true;
			node->Push(0, '0' - 1);
			node->Push('9' + 1, 127);
			GetNextToken();
			break;

		default: 
			Error("[ ] �ڴ���Ĵʷ���Ԫ");

		}
		if (!range)
		{
			GetNextToken();
			if (Match(TokenType::MINUS))
			{
				GetNextToken();
				if (!Match(TokenType::WORD)  && !Match(TokenType::NOT_WORD) &&
					!Match(TokenType::SPACE) && !Match(TokenType::NOT_SPACE) &&
					!Match(TokenType::DIGIT) && !Match(TokenType::NOT_DIGIT))
				{
					if (token_.lexeme_ < lhs)
						Error("[ ] ���ַ���Χ����");
					node->Push(lhs, token_.lexeme_);
					GetNextToken();
				}
				else
				{
					Backoff();
					node->Push(lhs, lhs);
				}
			}
			else
				node->Push(lhs, lhs);
		}
	}
	// ���� ]
	GetNextToken();

	return node;
}

// ����ú�����ȷ�� ( ��ͷ
ASTNode* Parser::UnnameCapture()
{
	int count = count_++;
	// �� () ��Ϊ��ʱ
	if (Match(TokenType::RP))
	{
		GetNextToken();
		return new ASTUnnameCapture(new ASTEmpty(), count);
	}

	// �� () �ڲ�Ϊ��
	ASTNode* node = new ASTUnnameCapture(Regex(), count);
	if (!Match(TokenType::RP))
		Error("ȱ�� )");
	GetNextToken();
	return node;
}

// ����ú���ʱ��ȷ���� (?:
ASTNode* Parser::NotCapture()
{
	// �� (?:) ��Ϊ��ʱ
	if (Match(TokenType::RP))
	{
		GetNextToken();
		return new ASTEmpty();
	}
	
	// �� (?:) �ڲ�Ϊ��
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("ȱ�� )");
	GetNextToken();
	return node;
}

// ����ú�����ȷ���� (?<
ASTNode* Parser::NameCapture()
{
	std::string name = Name(); // Name ���� > ���˳�
	nameReferenceSet_.insert(name);
	GetNextToken();

	// �� (?<name>) ��Ϊ��ʱ
	if (Match(TokenType::RP))
	{
		GetNextToken();
		return new ASTNameCapture(new ASTEmpty(), name);
	}

	// �� (?<name>) �ڲ�Ϊ��ʱ
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("ȱ�� )");
	GetNextToken();
	return new ASTNameCapture(node, name);
}

// ����ú�����ȷ�� (?= ��ͷ
ASTNode* Parser::PositiveLookahead()
{
	// �� (?=) Ϊ��ʱ���ñ�һ��ͨ��
	if (Match(TokenType::RP))
	{
		//GetNextToken();
		//return new ASTEmpty();
		Error("Ԥ���ӱ��ʽ����Ϊ��");
	}

	// �� (?=) ��Ϊ��ʱ
	ASTNode* node = Regex();
	if (!Match(TokenType::RP))
		Error("ȱ�� )");
	GetNextToken();
	return new ASTPstLookahead(node);
}

// ����ú�����ȷ�� (?! ��ͷ
ASTNode* Parser::NegativeLookahead()
{
	// �� (?!) ��Ϊ��ʱ
	if (Match(TokenType::RP))
		Error("Ԥ���ӱ��ʽ����Ϊ��");

	// �� (?!) ��Ϊ��ʱ
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
	while (std::isdigit(token_.lexeme_))
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

bool Parser::IsAtomBegin(Token token)
{
	switch (token.type_)
	{
	case TokenType::SIMPLECHAR:
	case TokenType::ANY:
	case TokenType::NEGATE:
	case TokenType::DOLLAR:
	case TokenType::BOUND:
	case TokenType::NOT_BOUND:
	case TokenType::BACKREF:
	case TokenType::NAMEREF:
	case TokenType::LBRACKET:
	case TokenType::LP:
	case TokenType::WORD:
	case TokenType::NOT_WORD:
	case TokenType::SPACE:
	case TokenType::NOT_SPACE:
	case TokenType::DIGIT:
	case TokenType::NOT_DIGIT:
		return true;
	default:
		return false;
	}
}