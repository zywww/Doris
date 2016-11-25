#ifndef DORIS_TOKEN_H__
#define DORIS_TOKEN_H__

// 如果两个符号有不同语义，则应该给他设计一种 token
// 应该把 (?: (?= (?! \k<name> 等等分析为一个词法单元吗？看看别的实现
//
enum class TokenType
{
	SIMPLECHAR,		// 即用来匹配的ASCII字符
//	NUMBER,			// {min, max} 中的数字
//	WIDECHAR,		// 宽字符

	OR,				// |
	STAR,			// *
	PLUS,			// +
	QUERY,			// ? 即表示 0 或 1 次重复，也表示非贪婪匹配
	NEGATE,			// ^ 取反或者表行首和字符串首
	DOLLAR,			// $
	LBRACE,			// {
	RBRACE,			// }
	LBRACKET,		// [
	RBRACKET,		// ]
	LP,				// (   
	RP,				// )
	LANGLE,			// <
	RANGLE,			// >
	ANY,			// . 通配符 与除 \n 之外的任何单个字符匹配。
//	BACKSLASH,		// \ 反斜杠
	MINUS,			// - 连字符	若在 [] 外，则 MINUS 和 simplechar 的 '-' 是同语义的

	WORD,			// \w  ==  [a-zA-Z0-9_]
	NOT_WORD,		// \W  ==  [^a-zA-Z0-9_]
	SPACE,			// \s  ==  [ \f\n\r\t\v]
	NOT_SPACE,		// \S  ==  [^ \t\n]
	DIGIT,			// \d  ==  [0-9]
	NOT_DIGIT,		// \D  ==  [^0-9]

	BOUND,			// \b
	NOT_BOUND,      // \B

	BACKREF,		// \n 后向引用，n为一位数字，引用以匹配的分组号
	NAMEREF,		// \k 命名后向引用

	END
};

// TODO 要用 struct 还是 class
struct Token
{
	Token(TokenType type, char ch = '\0');

	TokenType		type_;
	char			lexeme_;	// 一般来说，只有匹配字符才会用到 lexeme_，简单字符只用到 1 个，
								// 还有 \number 的反向引用，将 number 存在 lexeme
};

bool operator==(const Token &lhs, const Token &rhs);

#endif
