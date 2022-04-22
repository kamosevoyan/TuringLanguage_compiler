#ifndef INCLUDEPARSER
#define INCLUDEPARSER 

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
	
	void get_global_symbols() &; 
	
	
	SwitchNode* parse_switch_statement(Node*) &;
	Node* parse_case_and_default_statement(Node*)&;
	DefaultNode* parse_default_statement(Node*)&; 
	
	Node* parse_if_statement(Node*)&;
	Node* parse_while_statement(Node*)&;
	Node* parse_do_while_statement(Node*)&;
	Node* parse_repeat_until_statement(Node*)&;
	Node* parse_block_statement(Node*)&;
	Node* parse_left_statement(Node*)&;
	Node* parse_right_statement(Node*)&;
	Node* parse_exit_statement(Node*)&;
	Node* parse_error_statement(Node*)&;
	Node* parse_write_statement(Node*)&;
	Node* parse_continue_statement(Node*)&;
	Node* parse_break_statement(Node*)&;


	
public:

	Parser();	
	MainNode* parse()&;	
};

#endif