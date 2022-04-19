#include "Lexer.h"


Lexer::Lexer()
{
	this->key_map["main"] = KEYWORDS::MAIN;
	this->key_map["while"] = KEYWORDS::WHILE;
	this->key_map["repeat"] = KEYWORDS::REPEAT;			
	this->key_map["until"] = KEYWORDS::UNTIL;
	this->key_map["do"] = KEYWORDS::DO;			
	this->key_map["if"] = KEYWORDS::IF;
	this->key_map["else"] = KEYWORDS::ELSE;
	this->key_map["write"] = KEYWORDS::WRITE;
	this->key_map["left"] = KEYWORDS::LEFT;
	this->key_map["right"] = KEYWORDS::RIGHT;
	this->key_map["exit"] = KEYWORDS::EXIT;
	this->key_map["error"] = KEYWORDS::ERROR;
	this->key_map["not"] = KEYWORDS::NOT;
	
	this->key_map["continue"] = KEYWORDS::CONTINUE;
	this->key_map["break"] = KEYWORDS::BREAK;			
	
	this->key_map["{"] = KEYWORDS::L_BR;
	this->key_map["}"] = KEYWORDS::R_BR;
	this->key_map["("] = KEYWORDS::L_PAR;
	this->key_map[")"] = KEYWORDS::R_PAR;				
	this->key_map[";"] = KEYWORDS::SEMICOLON;	
	this->key_map[","] = KEYWORDS::COMMA;
	
	
	this->key_map["function"] = KEYWORDS::FUNCTION;
	
	this->key_map["global_symbols"] = KEYWORDS::GLOBAL_SYMBOLS;
}
		
void Lexer::setStream(const std::stringstream& sstream)&
{
	this->sstream << sstream.str();
	this->correctStream();
}
			
void Lexer::correctStream()&
{
	std::string temp;
	temp = this->sstream.str();
	
	// Add spaces between these symbols to make our live easier
	temp = std::regex_replace(temp, std::regex("\\("), " ( ");
	temp = std::regex_replace(temp, std::regex("\\)"), " ) ");
	temp = std::regex_replace(temp, std::regex("\\{"), " { ");
	temp = std::regex_replace(temp, std::regex("\\}"), " } ");
	temp = std::regex_replace(temp, std::regex(","), " , ");
	temp = std::regex_replace(temp, std::regex(";"), " ; ");

	//removing comments
	temp = std::regex_replace(temp, std::regex("/\\*.*\\*.*"), "");
	
	// Make plain the entire text 
	//temp = std::regex_replace(temp, std::regex("\\n"), "  ");
	temp = std::regex_replace(temp, std::regex("\\r"), "  ");
	temp = std::regex_replace(temp, std::regex("\\t"), "  ");
	
	this->sstream.str(temp);
}

bool Lexer::is_correct_name(const std::string& name) &
{
	if (!isalpha(name[0]))
	{
		return false;
	}
	
	for (int i = 1; i < name.length(); ++i)
	{
		if (! (	(isalpha(name[i]) || (isdigit(name[i])))	))
		{
			return false;
		}
	}
	
	return true;
}

void Lexer::next_token()&
{
	std::string temp;	
	this->pos = this->sstream.tellg();	
	this->sstream >> temp;
	
	if (this->key_map.find(temp) != this->key_map.end())
	{
		this->keyword = this->key_map[temp];
		this->value = temp;
		return;
	}
	else
	if (temp.length() == 1)
	{
		this->keyword = KEYWORDS::SYMBOL;
		this->value = temp;
		return;
	}
	else
	if (temp.length() == 0)
	{
		this->keyword = KEYWORDS::END;
		this->value = "";
	}
	else
	{		
		this->keyword = KEYWORDS::NAME;
		
		if (!is_correct_name(temp))
		{
			throw std::string("\033[31mUnacceptable name " +  temp + "\n\033[0m");
		}
		
		this->value = temp;
		return;
	}
	
	return;
}

