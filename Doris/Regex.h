#ifndef DORIS_REGEX_H__
#define DORIS_REGEX_H__

class Regex
{
public:
	Regex(const char* regex);

	bool Parse();
	bool Match(const char* matchContent);
	bool Search();
	bool Replace();

private:
	const char* regex_
};

#endif 