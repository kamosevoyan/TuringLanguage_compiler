#ifndef LEXERINCLUDE
#define LEXERINCLUDE

#include <regex>     
#include <sstream>

class Lexer
{	
	friend class Parser;
	friend class Compiler;
		
private:
	enum class KEYWORDS  
{
	MAIN, REPEAT, UNTIL, DO, WHILE, IF, ELSE, WRITE, LEFT, RIGHT, EXIT, ERROR, L_BR, R_BR, L_PAR, R_PAR, 
	COLON, SEMICOLON,  SYMBOL, COMMA, END, CONTINUE, BREAK, NOT, FUNCTION, NAME, GLOBAL_SYMBOLS,
	SWITCH, CASE, DEFAULT
};
		
	KEYWORDS keyword;
	std::string value;
	int pos;
	std::stringstream sstream;
	std::string sstream_by_line = "";
	std::map<const std::string, KEYWORDS> key_map;
	
	bool is_correct_name(const std::string&)&;
		
public:

	Lexer();
	void setStream(const std::stringstream&)&;
	void correctStream()&;	
	void next_token()&;
};

#endif