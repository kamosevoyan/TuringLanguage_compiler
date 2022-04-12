#ifndef TURINGLANGUAGEINCLUDE
#define TURINGLANGUAGEINCLUDE

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <regex>
#include <map>
								
std::string strip(const std::string&);


enum class TYPE {MAIN, REPEAT_UNTIL, DO_WHILE, BLOCK, IF, IFNOT, IF_ELSE, 
								WHILE, LEFT, RIGHT, WRITE, EXIT, ERROR, CONTINUE, BREAK, SYMBOL_LIST};

class Lexer
{	
		friend class Parser;
		friend class Compiler;
		
	private:
		enum class KEYWORDS  {MAIN, REPEAT, UNTIL, DO, WHILE, IF, IFNOT, ELSE, WRITE, LEFT, RIGHT, EXIT, 
														ERROR, L_BR, R_BR, L_PAR, R_PAR, SEMICOLON,  SYMBOL, COMMA, END, CONTINUE, BREAK, NOT};
				
		KEYWORDS keyword;
		std::string value;
		int pos;
		
		std::map<const std::string, KEYWORDS> key_map;
		std::stringstream sstream;
		
	public:
		Lexer();
		
		void setStream(const std::stringstream&)&;
		
		void correctStream()&;
		
		void next_token()&;
};

class Node
{
		friend class Parser;
		friend class Compiler;
		friend void print(Node*, int);	
	
		TYPE type;
		Node* root = nullptr;
		
	public:
				
		Node(TYPE);
		
		virtual ~Node();
};
 
class SymbolsNode : public Node 
{
	friend class Parser;
	friend class Compiler;
	friend void print(Node*, int);		
		
	std::string symbols = "";
	bool has_not;
	
	public:
	
		SymbolsNode(TYPE);
		virtual ~SymbolsNode();
};

class PrimaryNode : public Node 
{
	public:
	
		PrimaryNode(TYPE);
		virtual ~PrimaryNode();
};

class WriteNode : public PrimaryNode
{
		friend class Parser;
		friend class Compiler;
		friend void print(Node*, int);	
				
		char symbol;
		
	public:
		
		WriteNode(TYPE);
		virtual ~WriteNode();
};	

class FlowControlNode : public Node
{	
		friend class Parser;
		friend class Compiler;
		friend void print(Node*, int);		
		
		Node* owner_loop = nullptr;
		
	public:		
	
		FlowControlNode(TYPE);		
		virtual ~FlowControlNode();
};

class StatementNode : public Node 
{	
		friend class Parser;
		friend class Compiler;
		friend void print(Node*, int);		
	
		SymbolsNode symbols = TYPE::SYMBOL_LIST;
		
	public:
		
		StatementNode(TYPE);
		virtual ~StatementNode();
};

class IfNode : public StatementNode 
{	
		friend class Parser;
		friend class Compiler;
		friend void print(Node*, int);		
	
		Node* statements;
		
	public:
		
		IfNode(TYPE);
		virtual ~IfNode();	
};

class IfElseNode : public StatementNode 
{	
		friend class Parser;
		friend class Compiler;
		friend void print(Node*, int);		
	
protected:
	
		Node* if_statements;
		Node* else_statements;
		
	
	public:
	
		IfElseNode(TYPE);		
		virtual ~IfElseNode();	
};

class LoopNode : public StatementNode
{
		friend class Parser;
		friend class Compiler;
		friend void print(Node*, int);		
		
		Node* statements;
		std::vector<int> continue_states;
		std::vector<int> break_states;
		
		int continue_state = 0;
		int break_state = 0;
		
	public:
		
		LoopNode(TYPE);		
		virtual ~LoopNode();			

};

class WhileNode : public LoopNode 
{
	public:
	
		WhileNode(TYPE);
		virtual ~WhileNode();
};

class DoWhileNode : public LoopNode 
{
	public:
	
		DoWhileNode(TYPE);
		virtual ~DoWhileNode();				
};

class RepeatUntilNode : public LoopNode 
{
	public:
	
		RepeatUntilNode(TYPE);
		virtual ~RepeatUntilNode();				
};

class BlockNode : public StatementNode 
{
	
		friend class Parser;
		friend class Compiler;
		friend void print(Node*, int);
	
		std::vector<Node*> statements;
		
	public:
		
		
		BlockNode(TYPE);
		virtual ~BlockNode();					
};

class MainNode : public StatementNode 
{
	
		friend class Parser;
		friend class Compiler;
		friend void print(Node*, int);	
	
		Node* statements;
		
	public:
				
		MainNode(TYPE);
		virtual ~MainNode();							
};

class Parser
{
	
	friend class Compiler;

	Lexer lexer;
	SymbolsNode globalSymbols = TYPE::SYMBOL_LIST;
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
	Node* parse_statement(Node*)&;	
	SymbolsNode parse_symbols_statement()&;	
	bool in_loop(Node*)&;
	
public:

	Parser();
	
	MainNode* parse()&;	
};

class Compiler
{
	Parser parser;	
	void compile_node(Node* , std::vector<std::string>& , size_t& , const std::string& )&;		
	void put_flow_controls(Node* , std::vector<std::string>& , size_t& , const std::string& )&;
	
public:
	
	Compiler(const std::stringstream&);		
	std::string compile()&;
};


void print(Node* , int  = 0);
void ui_handler(int, char*[]);
#endif