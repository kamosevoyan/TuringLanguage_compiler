#ifndef INCLUDEPARSER
#define INCLUDEPARSER

#include <map>
#include <iostream>

#include "Nodes.h"
#include "Lexer.h"

class Parser
{
	
	friend class Compiler;

private:

	Lexer lexer;
	SymbolsNode globalSymbols = TYPE::SYMBOL_LIST;
	std::map<const std::string, FunctionNode*> globalFunctions;
	
	std::vector<Lexer::KEYWORDS> tokens;
	std::vector<std::string> token_values;
	std::vector<int> poses;		
	int token_count = 0;
				
	void next_token()&;	
	Lexer::KEYWORDS offset_token(int)&;	
	std::string offset_token_value(int)&;	
	void get_all_tokens()&;	
	void setStream(const std::stringstream&)&;	
	
	
	MainNode* parse_main_statement()&;	
	FunctionNode* parse_function_statement()&;
	void parse_all_function_statements()&;
	Node* parse_statement(Node*)&;	
	SymbolsNode parse_symbols_statement()&;	
	
	bool in_loop(Node*)&;
	bool chek_if_has_exit_states(Node*)&;
	
	void get_global_symbols() &;
	
public:

	Parser();	
	MainNode* parse()&;	
};

#endif