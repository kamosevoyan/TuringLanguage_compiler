#ifndef LEXERINCLUDE
#define LEXERINCLUDE

#include <regex>
#include <sstream>

class Lexer
{	
		friend class Parser;
		friend class Compiler;
		
	private:
		enum class KEYWORDS  {MAIN, REPEAT, UNTIL, DO, WHILE, IF, IFNOT, ELSE, WRITE, LEFT, RIGHT, EXIT, 
							ERROR, L_BR, R_BR, L_PAR, R_PAR, SEMICOLON,  SYMBOL, COMMA, END, CONTINUE, BREAK, NOT, FUNCTION, NAME, GLOBAL_SYMBOLS,
							};
				
		KEYWORDS keyword;
		std::string value;
		int pos;
		
		std::map<const std::string, KEYWORDS> key_map;
		std::stringstream sstream;
		
		bool is_correct_name(const std::string&)&;
		
		
	public:
		Lexer();
		
		void setStream(const std::stringstream&)&;
		
		void correctStream()&;
		
		void next_token()&;
};

#endif