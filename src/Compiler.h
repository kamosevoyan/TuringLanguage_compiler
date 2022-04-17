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
	
public:
	
	Compiler(const std::stringstream&);		
	std::string compile()&;
};


#endif