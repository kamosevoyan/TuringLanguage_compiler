#ifndef TURINGLANGUAGEINCLUDE
#define TURINGLANGUAGEINCLUDE

#include <fstream> 
#include "Parser.h"

void print(Node* , int  = 0);

class Compiler
{
       
private:

	Parser parser;
	
	void compile_node(Node* , std::vector<std::string>& , size_t& , const std::string& )&;		
	void compile_global_functions(	std::map<const std::string, FunctionNode*> &, std::vector<std::string>& , size_t& , const std::string& )&;		
	void put_flow_controls(Node* , std::vector<std::string>& , size_t& , const std::string& )&;
	void compile_main_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_switch_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_repeat_until_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_do_while_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_while_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_if_else_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_if_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_left_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_right_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_exit_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_error_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_write_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_block_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_continue_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_break_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;
	void compile_function_call_node(Node*, std::vector<std::string>&, size_t&, const std::string&) &;

public:
	
	Compiler(const std::stringstream&);		
	std::string compile()&;
};

#endif