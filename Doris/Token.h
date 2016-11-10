#ifndef DORIS_TOKEN_H__
#define DORIS_TOKEN_H__

enum class TokenType
{
	SIMPLECHAR,		// 即用来匹配的ASCII字符
	WIDECHAR,		// 宽字符

	OR,				// |
	STAR,			// *
	PLUS,			// +
	QUERY,			// ? 即表示 0 或 1 次重复，也表示非贪婪匹配
	NEGATE,			// ^   取反或者表行首和字符串首
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

	BACKREF,		// \n 后向引用，n为一位数字，引用以匹配的分组号
	NAMEREF,		// \k 命名后向引用

	INTEGER			// 用于 {n, m}
};

// TODO 要用 struct 还是 class
class Token
{
public:
	Token(TokenType type, char ch1, char ch2);

private:
	TokenType		type_;
	char			lexeme_[2];	// 只有匹配字符才会用到 lexeme_，简单字符只用到 1 个，
								// 宽字符用到 2 个字符
};

#endif
