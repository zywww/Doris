#ifndef DORIS_TOKEN_H__
#define DORIS_TOKEN_H__

// ������������в�ͬ���壬��Ӧ�ø������һ�� token
// Ӧ�ð� (?: (?= (?! \k<name> �ȵȷ���Ϊһ���ʷ���Ԫ�𣿿������ʵ��
//
enum class TokenType
{
	SIMPLECHAR,		// ������ƥ���ASCII�ַ�
//	NUMBER,			// {min, max} �е�����
//	WIDECHAR,		// ���ַ�

	OR,				// |
	STAR,			// *
	PLUS,			// +
	QUERY,			// ? ����ʾ 0 �� 1 ���ظ���Ҳ��ʾ��̰��ƥ��
	NEGATE,			// ^ ȡ�����߱����׺��ַ�����
	DOLLAR,			// $
	LBRACE,			// {
	RBRACE,			// }
	LBRACKET,		// [
	RBRACKET,		// ]
	LP,				// (   
	RP,				// )
	LANGLE,			// <
	RANGLE,			// >
	ANY,			// . ͨ��� ��� \n ֮����κε����ַ�ƥ�䡣
//	BACKSLASH,		// \ ��б��
	MINUS,			// - ���ַ�	���� [] �⣬�� MINUS �� simplechar �� '-' ��ͬ�����

	WORD,			// \w  ==  [a-zA-Z0-9_]
	NOT_WORD,		// \W  ==  [^a-zA-Z0-9_]
	SPACE,			// \s  ==  [ \f\n\r\t\v]
	NOT_SPACE,		// \S  ==  [^ \t\n]
	DIGIT,			// \d  ==  [0-9]
	NOT_DIGIT,		// \D  ==  [^0-9]

	BOUND,			// \b
	NOT_BOUND,      // \B

	BACKREF,		// \n �������ã�nΪһλ���֣�������ƥ��ķ����
	NAMEREF,		// \k ������������

	END
};

// TODO Ҫ�� struct ���� class
struct Token
{
	Token(TokenType type, char ch = '\0');

	TokenType		type_;
	char			lexeme_;	// һ����˵��ֻ��ƥ���ַ��Ż��õ� lexeme_�����ַ�ֻ�õ� 1 ����
								// ���� \number �ķ������ã��� number ���� lexeme
};

bool operator==(const Token &lhs, const Token &rhs);

#endif
