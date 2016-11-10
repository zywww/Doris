#ifndef DORIS_TOKEN_H__
#define DORIS_TOKEN_H__

enum class TokenType
{
	SIMPLECHAR,		// ������ƥ���ASCII�ַ�
	WIDECHAR,		// ���ַ�

	OR,				// |
	STAR,			// *
	PLUS,			// +
	QUERY,			// ? ����ʾ 0 �� 1 ���ظ���Ҳ��ʾ��̰��ƥ��
	NEGATE,			// ^   ȡ�����߱����׺��ַ�����
	DOLLAR,			// $
	LBRACE,			// {
	RBRACE,			// }
	LBRACKET,		// [
	RBRACKET,		// ]
	LP,				// (   
	RP,				// )

	WORD,			// \w  ==  [a-zA-Z0-9_]
	NOT_WORD,		// \W  ==  [^a-zA-Z0-9_]
	SPACE,			// \s  ==  [ \t\n]
	NOT_SPACE,		// \S  ==  [^ \t\n]
	DIGIT,			// \d  ==  [0-9]
	NOT_DIGIT,		// \D  ==  [^0-9]

	BOUND,			// \b
	NOT_BOUND,      // \B

	BACKREF,		// \n �������ã�nΪһλ���֣�������ƥ��ķ����
	NAMEREF,		// \k ������������

	INTEGER			// ���� {n, m}
};

// TODO Ҫ�� struct ���� class
class Token
{
public:
	Token(TokenType type, char ch1, char ch2);

private:
	TokenType		type_;
	char			lexeme_[2];	// ֻ��ƥ���ַ��Ż��õ� lexeme_�����ַ�ֻ�õ� 1 ����
								// ���ַ��õ� 2 ���ַ�
};

#endif
