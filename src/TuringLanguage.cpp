#include "TuringLanguage.h"
#define DEBUG
			
std::string strip(const std::string& input)
{
	std::string output(input);
	std::size_t pos = 0;
	
	while( ((pos = output.find(" ")) != std::string::npos)   ||  ((pos = output.find("\t")) != std::string::npos) )
	{
		output.erase(pos, 1);
	}
	return output;
}

void ui_handler(int argc, char* argv[])
{
	if (argc < 3)
	{
		throw std::string("\033[31mError: missing inputs: Use following pattern: /TuringLanguage input_file_name\toutput_file_name\n\033[0m");
	}
	
	if (argc > 3)
	{
		throw std::string("\033[31mError: to many inputs: Use following pattern: /TuringLanguage input_file_name\toutput_file_name\n\033[0m");
	}
		
	std::ifstream file;
	file.open(argv[1]);
	
	if (file.fail())
	{
		throw std::string("\033[31mError: invalid input file name or directory\n\033[0m");
	}
	
	std::ofstream output_file_name(argv[2]);

	if (output_file_name.fail())
	{
		throw std::string("\033[31mError: invalid output file name or directory\n\033[0m");
	}
	
	std::stringstream sstream;
	sstream << file.rdbuf();	
	file.close();
		
	Compiler compiler(sstream);

	std::string source_code =  compiler.compile();
	
	output_file_name << source_code;
	output_file_name.close();
}

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
	
	
	// Make plain the entire text 
	//temp = std::regex_replace(temp, std::regex("\\n"), "  ");
	temp = std::regex_replace(temp, std::regex("\\r"), "  ");
	temp = std::regex_replace(temp, std::regex("\\t"), "  ");
	
	this->sstream.str(temp);
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
		throw  std::string("\033[31mUnknown syntax token " +  temp + "\n\033[0m");
	}
	
	return;
}
			
Node::Node(TYPE type):type(type)
{
			
}
		
Node::~Node()
{
#ifdef DEBUG			
	std::cout << "Node destructor called\n";			
#endif
}

SymbolsNode::SymbolsNode(TYPE type):Node(type)
{
	
}

SymbolsNode::~SymbolsNode()
{
#ifdef DEBUG			
	std::cout << "SymbolsNode destructor called\n";			
#endif
}

PrimaryNode::PrimaryNode(TYPE type):Node(type)
{
	
}

PrimaryNode::~PrimaryNode()
{
#ifdef DEBUG			
	std::cout << "PrimaryNode destructor called\n";			
#endif
}

WriteNode::WriteNode(TYPE type):PrimaryNode(type)
{
	
}

WriteNode::~WriteNode()
{
#ifdef DEBUG			
	std::cout << "WriteNode destructor called\n";			
#endif
}

FlowControlNode::FlowControlNode(TYPE type):Node(type)
{
	
}

FlowControlNode::~FlowControlNode()
{
#ifdef DEBUG			
	std::cout << "FlowControlNode destructor called\n";			
#endif
}

StatementNode::StatementNode(TYPE type):Node(type)
{
	
}

StatementNode::~StatementNode()
{
#ifdef DEBUG			
	std::cout << "StatementNode destructor called\n";			
#endif			
}

IfNode::IfNode(TYPE type):StatementNode(type)
{
	
}

IfNode::~IfNode()
{
#ifdef DEBUG						
	std::cout << "Ifnode destructor called\n";
#endif			
	delete this->statements;
}		

IfElseNode::IfElseNode(TYPE type):StatementNode(type)
{
	
}

IfElseNode::~IfElseNode()
{
#ifdef DEBUG						
	std::cout << "IfElseNode destructor called\n";
#endif			
	delete this->if_statements;
	delete this->else_statements;
}					

LoopNode::LoopNode(TYPE type):StatementNode(type)
{
	
}

LoopNode::~LoopNode()
{
#ifdef DEBUG						
	std::cout << "LoopNode destructor called\n";
#endif
	delete this->statements;
}				

WhileNode::WhileNode(TYPE type):LoopNode(type)
{
	
}

WhileNode::~WhileNode()
{
#ifdef DEBUG						
	std::cout << "WhileNode destructor called\n";
#endif
}				
		
DoWhileNode::DoWhileNode(TYPE type):LoopNode(type)
{
	
}

DoWhileNode::~DoWhileNode()
{
#ifdef DEBUG						
	std::cout << "DoWhileNode destructor called\n";
#endif			
}				
		
RepeatUntilNode::RepeatUntilNode(TYPE type):LoopNode(type)
{
	
}

RepeatUntilNode::~RepeatUntilNode()
{
#ifdef DEBUG						
	std::cout << "RepeatUntilNode destructor called\n";
#endif			
}				

BlockNode::BlockNode(TYPE type):StatementNode(type)
{
	
}

BlockNode::~BlockNode()
{
#ifdef DEBUG						
	std::cout << "BlockNode destructor called\n";
#endif			
	
	for (int i = 0; i < this->statements.size(); ++i)
	{
		delete this->statements[i];
	}
}							

MainNode::MainNode(TYPE type):StatementNode(type)
{
	
}

MainNode::~MainNode()
{
#ifdef DEBUG						
	std::cout << "MainNode destructor called\n";
#endif			
	delete this->statements;
}								

Parser::Parser()
{
	
}		

void Parser::next_token()&
{
	this->token_count += 1;
}
		
Lexer::KEYWORDS Parser::offset_token(int offset)&
{
	if ( (this->token_count + offset <(int)this->tokens.size())  && (this->token_count + offset >= 0) )
	{
		return this->tokens[this->token_count + offset];
	}
	else
	{
		return this->tokens[this->token_count];				
	}
}
		
std::string Parser::offset_token_value(int offset) &
{
	if ( (this->token_count + offset < (int)this->tokens.size())  && (this->token_count + offset >= 0) )
	{
		return this->token_values[this->token_count + offset];
	}
	else
	{
		return this->token_values[this->token_count];				
	}
}
		
void Parser::get_all_tokens() &
{
	do
	{
		this->tokens.push_back(this->lexer.keyword);
		this->token_values.push_back(this->lexer.value);
		this->poses.push_back(this->lexer.pos);
		
		this->lexer.next_token();
	}
	while(this->lexer.keyword != Lexer::KEYWORDS::END);
	
	this->tokens.push_back(this->lexer.keyword);
	this->token_values.push_back(this->lexer.value);
}
		
void Parser::setStream(const std::stringstream& sstream) &	
{
	this->lexer.setStream(sstream);
	get_all_tokens();
}
		
MainNode* Parser::parse() &
{
	this->next_token();
	MainNode* node = parse_main_statement();
	node->root = nullptr;
	
	this->next_token();
	
	if (this->offset_token(0) != Lexer::KEYWORDS::END)
	{
		throw std::string("\033[31mInvalid syntax " + this->offset_token_value(0) + "\n\033[0m");
	}
	return node;
}
		
MainNode* Parser::parse_main_statement() &
{
		
	if (this->offset_token(0) != Lexer::KEYWORDS::MAIN)
	{
		throw std::string("\033[31mExpected main statement\n\033[0m");
	}
	
#ifdef DEBUG			
	std::cout << "Main statement begin\n";
#endif			

	MainNode* node = new MainNode(TYPE::MAIN);
	
	//this->next_token();
	node->symbols = parse_symbols_statement();
	this->globalSymbols = node->symbols;

	this->next_token();
	
	if (this->offset_token(0) != Lexer::KEYWORDS::L_BR)
	{
		throw std::string("\033[31mMain statement should only contain block statement.\n\033[0m");
	}
	
	node->statements = parse_statement(node);		

#ifdef DEBUG								
	std::cout << "Main statement end\n";
#endif						
	return node;
}

Node* Parser::parse_statement(Node* root) &
{	
	Node* node0 = nullptr;
	Node* node1 = nullptr;
	
	if (this->offset_token(0) == Lexer::KEYWORDS::IF)
	{				
		IfNode* first_node = new IfNode(TYPE::IF);
		first_node->root = root;

#ifdef DEBUG							
		std::cout << "If statement begin\n";
#endif				
		SymbolsNode symnode = parse_symbols_statement();
		
		this->next_token();				
		node0 = parse_statement(first_node);
		
		if (this->offset_token(1) == Lexer::KEYWORDS::ELSE)
		{
#ifdef DEBUG												
			std::cout << "Else statement begin\n";
#endif					
			this->next_token();
			this->next_token();
			
			first_node->statements = nullptr;
			delete first_node;
								
			IfElseNode* node = new IfElseNode(TYPE::IF_ELSE);
			node->root = root;
			
			node1 = parse_statement(node);
			node0->root = node;
								
			node->if_statements = node0;
			node->else_statements = node1;
			node->symbols = symnode;
			
#ifdef DEBUG												
			std::cout << "Else statement end\n";
#endif						
			
			return node;
		}
										
		first_node->statements = node0;
		first_node->symbols = symnode;			

#ifdef DEBUG							
		std::cout << "if statement end\n";
#endif								
		return first_node;
	}		
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::WHILE)
	{
		WhileNode* node = new WhileNode(TYPE::WHILE);
		node->root = root;

#ifdef DEBUG											
		std::cout << "While statement begin\n";
#endif
		
		SymbolsNode symnode = parse_symbols_statement();
		
		this->next_token();
		
		node0 = parse_statement(node);
		
		node->statements = node0;
		node->symbols = symnode;

#ifdef DEBUG											
		std::cout << "While statement end\n";
#endif				

		return node;
	}
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::DO)
	{
		DoWhileNode* node = new DoWhileNode(TYPE::DO_WHILE);
		node->root = root;

#ifdef DEBUG											
		std::cout << "DoWhile statement begin\n";
#endif				
		
		this->next_token();
		
		node0 = parse_statement(node);
		
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::WHILE)
		{
			throw std::string("\033[31mIncorrect Do/While statement\n\033[0m");
		}
		
		SymbolsNode symnode = parse_symbols_statement();
		
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon	 \n\033[0m");
		}		
		
		node->statements = node0;
		node->symbols = symnode;
		
#ifdef DEBUG															
		std::cout << "DoWhile statement end\n";
#endif				

		return node;
	}
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::REPEAT)
	{
		RepeatUntilNode* node = new RepeatUntilNode(TYPE::REPEAT_UNTIL);
		node->root = root;

#ifdef DEBUG											
		std::cout << "RepeatUntil statement begin\n";
#endif				
		
		this->next_token();
		
		node0 = parse_statement(node);
		
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::UNTIL)
		{
			throw std::string("\033[31mIncorrect Repeat/Until statement\n\033[0m");
		}
		
		SymbolsNode symnode = parse_symbols_statement();
		
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon	 \n\033[0m");
		}		
		
		node->statements = node0;
		node->symbols = symnode;

#ifdef DEBUG															
		std::cout << "RepeatUntil statement end\n";
#endif				

		return node;
	}			
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::L_BR)
	{

		BlockNode* node = new BlockNode(TYPE::BLOCK);
		node->root = root;
		
		this->next_token();

		if ( this->offset_token(0) == Lexer::KEYWORDS::R_BR)
		{
			throw std::string("\033[31mEmpty block statement\n\033[0m");
		}

#ifdef DEBUG											
		std::cout << "Block statement begin\n";
#endif				
		
		while (this->offset_token(0)  != Lexer::KEYWORDS::R_BR)
		{
				node0 = parse_statement(node);
				node->statements.insert(node->statements.end(), node0);
				this->next_token();
		}
		
		// Add an aditional exit state when last node isn't exit or error to prevent simulator error.
		if (static_cast<BlockNode*>(node)->root->type == TYPE::MAIN)
		{
			if ( (static_cast<BlockNode*>(node)->statements[static_cast<BlockNode*>(node)->statements.size() - 1]->type != TYPE::EXIT) &&
				 (static_cast<BlockNode*>(node)->statements[static_cast<BlockNode*>(node)->statements.size() - 1]->type != TYPE::ERROR))
			{
				node->statements.insert(node->statements.end(), new PrimaryNode(TYPE::EXIT));
			}
		}
		
#ifdef DEBUG											
		std::cout << "Block statement end\n";
#endif				
		
		return node;
		
	}
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::LEFT)
	{
		
#ifdef DEBUG											
		std::cout << "statement left case end\n";
#endif				
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon\n\033[0m");
		}				
		
		return new PrimaryNode(TYPE::LEFT);
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::RIGHT)
	{
#ifdef DEBUG											
		std::cout << "statement right case end\n";
#endif				
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon\n\033[0m");
		}				
		
		return new PrimaryNode(TYPE::RIGHT);
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::EXIT)
	{
#ifdef DEBUG											
		std::cout << "statement EXIT case end\n";
#endif				
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon\n\033[0m");
		}				
		
		return new PrimaryNode(TYPE::EXIT);
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::ERROR)
	{
#ifdef DEBUG											
		std::cout << "statement ERROR case end\n";
#endif				
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon\n\033[0m");
		}				
		
		return new PrimaryNode(TYPE::ERROR);
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::WRITE)
	{
#ifdef DEBUG											
		std::cout << "Writes state begin\n";
#endif				
		
		SymbolsNode symnode = parse_symbols_statement();
		
		if (symnode.symbols.length() > 1)
		{
			throw std::string("\033[31mExpected one symbol in write statement\n\033[0m");
		}
		
		WriteNode* node= new WriteNode(TYPE::WRITE);
		node->symbol = symnode.symbols[0];
		
		this->next_token();
		
		if (this->offset_token(0) != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon\n\033[0m");
		}
		
		return node;				
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::CONTINUE)
	{
#ifdef DEBUG											
		std::cout << "statement continue case end\n";
#endif				
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon\n\033[0m");
		}				
		
		FlowControlNode* node = new FlowControlNode(TYPE::CONTINUE); 
		node->root = root;
		
		if (!	in_loop(node))
		{
			throw std::string("\033[31mFlow control statement continue should be within a owner_loop\n\033[0m");
		}
		
		return node;
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::BREAK)
	{
#ifdef DEBUG											
		std::cout << "statement continue case end\n";
#endif				
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon\n\033[0m");
		}				
		
		FlowControlNode* node = new FlowControlNode(TYPE::BREAK); 
		node->root = root;
		
		if (!	in_loop(node))
		{
			throw std::string("\033[31mFlow control statement continue should be within a owner_loop\n\033[0m");
		}
		
		return node;					
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::SEMICOLON)
	{
		throw std::string("\033[31mSemicolon shoul follow by an primary statement \n\033[0m");
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::R_BR)
	{
		throw  std::string("\033[31mEmpty single statement \n\033[0m");
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::ELSE)
	{
		throw std::string("\033[31mElse statement should follow by an if statement\n\033[0m");
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::END)
	{
		throw std::string("\033[31mExpected statement\n\033[0m");
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::MAIN)
	{
		throw  std::string("\033[31mMain statement should occur only once \n\033[0m");
	}
	else
	{						
		throw std::string("\033[31mUnknow statement " + this->offset_token_value(0) + "\n\033[0m");
	}			

}

SymbolsNode Parser::parse_symbols_statement() &
{			
	SymbolsNode node = TYPE::SYMBOL_LIST;
	bool isGlobal;
	bool has_not;
	
	if (this->globalSymbols.symbols.length() == 0)
	{
		isGlobal = true;
	}
	else
	{
		isGlobal = false;
	}	
	
	//this->next_token();
	
	if (this->offset_token(1) == Lexer::KEYWORDS::NOT)
	{
		this->next_token();
		
		if (isGlobal)
		{
			throw std::string("\033[31mMain statement should not have not attribute.\n\033[0m");
		}
		has_not = true;
		std::cout << "hasss nooot \n";
	}
	else
	{
		has_not = false;
	}
	
	this->next_token();
	
	if (this->offset_token(0)  != Lexer::KEYWORDS::L_PAR)
	{
		throw std::string("\033[31mExpected left parenrhesis\n\033[0m");
	}
	
	bool las_was_comma = false;
	bool first_was_symbol = false;
					
	while (true)
	{
		this->next_token();
		
		
		if (this->offset_token(0)  == Lexer::KEYWORDS::R_PAR)
		{
			if (this->offset_token(-1) == Lexer::KEYWORDS::L_PAR)
			{
				throw std::string("\033[31mEmpty parenthesis statement\n\033[0m");
			}
			break;
		}
		
		if ((this->offset_token(0)  != Lexer::KEYWORDS::SYMBOL) && ( ! first_was_symbol))
		{
			throw std::string("\033[31mExpected symbol before comma\n\033[0m");
		}
		
		first_was_symbol = true;
		
		if (this->offset_token(0)  == Lexer::KEYWORDS::SYMBOL)
		{
			
			if (std::find(node.symbols.begin(), node.symbols.end(), this->offset_token_value(0)[0]) != node.symbols.end())
			{
				
				throw std::string("\033[31mSymbol " + this->offset_token_value(0) + " is already given \n\033[0m");
			}
			
			if (!isGlobal)
			{
				if (std::find(this->globalSymbols.symbols.begin(), this->globalSymbols.symbols.end(), this->offset_token_value(0)[0]) == this->globalSymbols.symbols.end())
				{
					throw std::string("\033[31mThe symbol " + this->offset_token_value(0) + " is not given in main symbols\n\033[0m");
				}
			}
			
			
			node.symbols.push_back(this->offset_token_value(0)[0]);			
			las_was_comma = false;
		}
		else
		if (this->offset_token(0)  == Lexer::KEYWORDS::COMMA)
		{
			las_was_comma = true;
			continue;
		}
		
		else
		{
			throw std::string("\033[31mExpected symbol \n\033[0m");
		}
		
	}
	
		if (las_was_comma)
		{
			throw std::string("\033[31mExpected symbol after comma \n\033[0m");
		}
		
		node.has_not = has_not;
		
		return node;
		
}

bool Parser::in_loop(Node* node) &
{
	Node* it = node;
	
	while(!((it->root->type == TYPE::WHILE) || (it->root->type == TYPE::DO_WHILE) ||
				(it->root->type == TYPE::REPEAT_UNTIL) || (it->root->type == TYPE::MAIN)))
	{
		it = it->root;
	}
	
	if (it->root->type == TYPE::MAIN)
	{
		return false;
	}
				
	static_cast<FlowControlNode*>(node)->owner_loop = it->root;									
	return true;						
}

void print(Node* node, int shift)
{
	if (node->type == TYPE::BREAK)
	{
		std::cout << "break";
	}
	else
	if (node->type == TYPE::CONTINUE)
	{
		std::cout << "continue";
	}
	else	
	if (node->type == TYPE::LEFT)
	{
		std::cout << "left";
	}
	else
	if (node->type == TYPE::RIGHT)
	{
		std::cout << "right";
	}
	else
	if (node->type == TYPE::EXIT)
	{
		std::cout << "exit";
	}
	else
	if (node->type == TYPE::ERROR)
	{
		std::cout << "error";
	}
	else
	if (node->type == TYPE::MAIN)
	{
		std::cout << "main\n{\n";
		
		for (int i = 0; i < shift + 1; ++i)
		{
				std::cout << " ";
		}
		print(((MainNode*)node)->statements, shift + 1);
		std::cout << "	\n}\n";
	}
	else
	if (node->type == TYPE::BLOCK)
	{
		std::cout << "block\n";
		
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}
		
		std::cout << "{\n";
		
		for (int j = 0; j < ((BlockNode*)node)->statements.size() - 1; ++j)
		{
			for (int i = 0; i < shift + 1; ++i)
			{
					std::cout << " ";
			}
			print(((BlockNode*)node)->statements[j], shift + 1);
			std::cout << "\n";
		}
		
		for (int i = 0; i < shift + 1; ++i)
		{
				std::cout << " ";
		}
		print(((BlockNode*)node)->statements[((BlockNode*)node)->statements.size() - 1], shift + 1);		
		
		std::cout << "\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}				
		std::cout << "}";
	}
	else
	if (node->type == TYPE::IF)
	{
		std::cout << "if\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}
		
		std::cout << "{\n";
		
		for (int i = 0; i < shift + 1; ++i)
		{
				std::cout << " ";
		}

		print(((IfNode*)node)->statements, shift + 1);
		
		std::cout << "\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}				
		std::cout << "}";

	}
	else
	if (node->type == TYPE::IF_ELSE)
	{
		std::cout << "if\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}
		
		std::cout << "{\n";
		
		for (int i = 0; i < shift + 1; ++i)
		{
				std::cout << " ";
		}

		print(((IfElseNode*)node)->if_statements, shift + 1);
		
		std::cout << "\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}				
		std::cout << "}\n";
		
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}
		
		std::cout << "else\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}
		
		std::cout << "{\n";
		
		for (int i = 0; i < shift + 1; ++i)
		{
				std::cout << " ";
		}

		print(((IfElseNode*)node)->else_statements, shift + 1);
		
		std::cout << "\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}				
		std::cout << "}";		

	}
	else
	if (node->type == TYPE::WHILE)
	{
		std::cout << "while\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}
		
		std::cout << "{\n";
		
		for (int i = 0; i < shift + 1; ++i)
		{
				std::cout << " ";
		}

		print(((WhileNode*)node)->statements, shift + 1);
		
		std::cout << "\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}				
		std::cout << "}";

	}
	else
	if (node->type == TYPE::REPEAT_UNTIL)
	{
		std::cout << "until\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}
		
		std::cout << "{\n";
		
		for (int i = 0; i < shift + 1; ++i)
		{
				std::cout << " ";
		}

		print(((RepeatUntilNode*)node)->statements, shift + 1);
		
		std::cout << "\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}				
		std::cout << "}";

	}
	else
	if (node->type == TYPE::DO_WHILE)
	{
		std::cout << "do_while\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}
		
		std::cout << "{\n";
		
		for (int i = 0; i < shift + 1; ++i)
		{
				std::cout << " ";
		}

		print(((DoWhileNode*)node)->statements, shift + 1);
		
		std::cout << "\n";
		for (int i = 0; i < shift ; ++i)
		{
				std::cout << " ";
		}				
		std::cout << "}";

	}			
}
	
Compiler::Compiler(const std::stringstream& sstream)
{
	this->parser.setStream(sstream);
}

std::string Compiler::compile() &
{
	MainNode* node;

	node = this->parser.parse();
	std::string symbols = this->parser.globalSymbols.symbols;
	
	std::vector<std::string> lines;
	std::string temp = "";
	
	size_t state_number = 0;
					
	compile_node(node, lines, state_number, symbols);
					
#ifdef DEBUG															
	print(node);				
#endif				
	
	temp += symbols[0];
	for (int i = 1; i < symbols.length(); ++i)
	{
		temp += "," + symbols.substr(i,1);
	}
	temp+="\n";
	
	temp += "q0";
	for (int i = 1; i < state_number; ++i)
	{
		temp += ",q" + std::to_string(i);
	}
	temp+=",h\n";
	
	for (int i = 0; i < lines.size(); ++i)
	{
		temp+= lines[i] ;
	}
	
	temp += "q0\nh\n_\n";
					
	delete node;			
	return temp;			
}

void Compiler::compile_node(Node* node, std::vector<std::string>& lines, size_t& state_number, const std::string& globalSymbols) &
{
	std::string temp = "";
	int curent_line;
	size_t old_state_number;
	
	switch (node->type)
	{
		case	TYPE::LEFT:
		
				++state_number;
				temp += globalSymbols.substr(0,1) + ",q" + std::to_string(state_number) + ",<";
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					temp += std::string("\t|\t") +  globalSymbols.substr(i,1) + ",q" + std::to_string(state_number) + ",<";
				}
				temp += "\n";
				lines.push_back(temp);
				
		break;
		
		case	TYPE::RIGHT:
		
				++state_number;
				temp += globalSymbols.substr(0,1) + ",q" + std::to_string(state_number) + ",>";
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					temp += std::string("\t|\t") +  globalSymbols.substr(i,1) + ",q" + std::to_string(state_number) + ",>";
				}
				temp += "\n";
				lines.push_back(temp);		
				
		break;
		
		case	TYPE::EXIT:
				
				++state_number;
				temp += globalSymbols.substr(0,1) + ",h,@";
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					temp += std::string("\t|\t") +  globalSymbols.substr(i,1) + ",h,@";
				}
				temp += "\n";
				lines.push_back(temp);		
				
		break;		

		case	TYPE::ERROR:
		
				++state_number;
				temp += "X";
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					temp += std::string("\t|\t") +  "X";
				}
				temp += "\n";
				lines.push_back(temp);						
				
		break;								

		case	TYPE::CONTINUE:
		
				++state_number;
				lines.push_back("");	
				static_cast<LoopNode*>(static_cast<FlowControlNode*>(node)->owner_loop)->continue_states.push_back(state_number);				
				
		break;
		
		case	TYPE::BREAK:
		
				++state_number;
				lines.push_back("");	
				static_cast<LoopNode*>(static_cast<FlowControlNode*>(node)->owner_loop)->break_states.push_back(state_number);				
				
		break;				
		
		case	TYPE::WRITE:
		
				++state_number;
				temp = static_cast<WriteNode*>(node)->symbol;
				temp += ",q" + std::to_string(state_number) + ",@";
				
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					temp += std::string("\t|\t") + static_cast<WriteNode*>(node)->symbol + ",q" + std::to_string(state_number) + ",@";
				}
				temp += "\n";
				lines.push_back(temp);			
				
		break;		
		
		case	TYPE::MAIN:
		
				compile_node(static_cast<MainNode*>(node)->statements, lines, state_number, globalSymbols);
				
		break;
		
		case	TYPE::BLOCK:
		
				for (int i = 0; i < static_cast<BlockNode*>(node)->statements.size(); ++i)
				{
					compile_node(static_cast<BlockNode*>(node)->statements[i], lines, state_number, globalSymbols);
				}
										
		break;
		
		case	TYPE::IF:
		{
				++state_number;
				old_state_number = state_number;
				
				lines.push_back("");
				curent_line = lines.size() - 1;
				bool has_not = static_cast<IfNode*>(node)->symbols.has_not;
				
				compile_node(static_cast<IfNode*>(node)->statements, lines, state_number, globalSymbols);
				
				if (has_not == true)
				std::cout << "if has not \n";
				
				if (  (has_not) ^ (static_cast<IfNode*>(node)->symbols.symbols.find(globalSymbols[0]) != std::string::npos) )
				{
					temp += globalSymbols.substr(0,1) + ",q" + std::to_string(old_state_number ) + ",@";
				}
				else
				{
					temp += globalSymbols.substr(0,1) + ",q" + std::to_string(state_number ) + ",@";
				}	
				
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					if (  (has_not)  ^ ( static_cast<IfNode*>(node)->symbols.symbols.find(globalSymbols[i]) != std::string::npos) )
					{
						temp += std::string("\t|\t") + globalSymbols.substr(i,1)+ ",q" + std::to_string(old_state_number ) + ",@";
					}
					else
					{
						temp += std::string("\t|\t") + globalSymbols.substr(i,1) + ",q" + std::to_string(state_number ) + ",@";
					}	
				}
				temp += "\n";							
				lines[curent_line] = temp;
		}	
		break;	
		
		case	TYPE::IF_ELSE:
		{
			   ++state_number;
				old_state_number = state_number;
				
				lines.push_back("");
				curent_line = lines.size() - 1;
				bool has_not = static_cast<IfElseNode*>(node)->symbols.has_not;				
				
				compile_node(((IfElseNode*)node)->if_statements, lines, state_number, globalSymbols);
				lines.push_back("");
				
				++state_number;		
				size_t else_old_state_number = state_number;							
				size_t else_current_line = lines.size() - 1;
				
				
				compile_node(((IfElseNode*)node)->else_statements, lines, state_number, globalSymbols);
				
				
				if ( has_not ^ (static_cast<IfElseNode*>(node)->symbols.symbols.find(globalSymbols[0]) != std::string::npos))
				{
					temp += globalSymbols.substr(0,1) + ",q" + std::to_string(old_state_number ) + ",@";
				}
				else
				{
					temp += globalSymbols.substr(0,1) + ",q" + std::to_string(else_old_state_number ) + ",@";
				}	
				
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					if ( has_not ^ (static_cast<IfElseNode*>(node)->symbols.symbols.find(globalSymbols[i]) != std::string::npos))
					{
						temp += std::string("\t|\t") + globalSymbols.substr(i,1)+ ",q" + std::to_string(old_state_number ) + ",@";
					}
					else
					{
						temp += std::string("\t|\t") + globalSymbols.substr(i,1) + ",q" + std::to_string(else_old_state_number ) + ",@";
					}	
				}
				
				temp += "\n";					
				
				lines[curent_line] = temp;						
				temp = "";												
				temp += globalSymbols.substr(0,1) + ",q" + std::to_string(state_number ) + ",@";
										
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					temp += std::string("\t|\t") + globalSymbols.substr(i,1)+ ",q" + std::to_string(state_number ) + ",@";
				}
				
				temp += "\n";						
				lines[else_current_line] = temp;
				
		}
		break;
		
		case	TYPE::WHILE:			
		{
				++state_number;
				old_state_number = state_number;
				
				lines.push_back("");
				curent_line = lines.size() - 1;
				bool has_not = static_cast<IfElseNode*>(node)->symbols.has_not;								
				
				compile_node(((WhileNode*)node)->statements, lines, state_number, globalSymbols);
				
				
				if ( has_not ^  (static_cast<WhileNode*>(node)->symbols.symbols.find(globalSymbols[0]) != std::string::npos))
				{
					temp += globalSymbols.substr(0,1) + ",q" + std::to_string(old_state_number ) + ",@";
				}
				else
				{
					temp += globalSymbols.substr(0,1) + ",q" + std::to_string(state_number  + 1) + ",@";
				}	
				
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					if (has_not ^ (static_cast<WhileNode*>(node)->symbols.symbols.find(globalSymbols[i]) != std::string::npos))
					{
						temp += std::string("\t|\t") + globalSymbols.substr(i,1)+ ",q" + std::to_string(old_state_number ) + ",@";
					}
					else
					{
						temp += std::string("\t|\t") + globalSymbols.substr(i,1) + ",q" + std::to_string(state_number  + 1) + ",@";
					}	
				}
											
				temp += "\n";							
				lines[curent_line] = temp;
																		
				temp = "";
				++state_number;
				 
				temp += globalSymbols.substr(0,1) + ",q" + std::to_string(old_state_number - 1) + ",@";
				
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					temp +=  std::string("\t|\t") + globalSymbols.substr(i,1)  + ",q" + std::to_string(old_state_number - 1) + ",@";
				}
				
				temp += "\n";
				
				lines.push_back(temp);
				
				static_cast<WhileNode*>(node)->continue_state = old_state_number - 1;
				static_cast<WhileNode*>(node)->break_state = state_number;				

				put_flow_controls(node, lines, state_number, globalSymbols);
		}		
		break;				
						
		case	TYPE::DO_WHILE:
		{
				old_state_number = state_number;							
				bool has_not = static_cast<IfElseNode*>(node)->symbols.has_not;				
				
				compile_node(static_cast<DoWhileNode*>(node)->statements, lines, state_number, globalSymbols);
				
				++state_number;										
				
				if (has_not ^  (static_cast<DoWhileNode*>(node)->symbols.symbols.find(globalSymbols[0]) != std::string::npos))
				{
					temp += globalSymbols.substr(0,1) + ",q" + std::to_string(old_state_number) + ",@";
				}
				else
				{
					temp += globalSymbols.substr(0,1) + ",q" + std::to_string(state_number ) + ",@";
				}	
				
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					if (has_not ^ (static_cast<DoWhileNode*>(node)->symbols.symbols.find(globalSymbols[i]) != std::string::npos))
					{
						temp += std::string("\t|\t") + globalSymbols.substr(i,1)+ ",q" + std::to_string(old_state_number) + ",@";
					}
					else
					{
						temp += std::string("\t|\t") + globalSymbols.substr(i,1) + ",q" + std::to_string(state_number ) + ",@";
					}	
				}
											
				temp += "\n";							
				lines.push_back(temp);	
				
				static_cast<DoWhileNode*>(node)->continue_state = old_state_number ;
				static_cast<DoWhileNode*>(node)->break_state = state_number;				

				put_flow_controls(node, lines, state_number, globalSymbols);							
				
		}						
		break;					
		
		case	TYPE::REPEAT_UNTIL:
		{
				old_state_number = state_number;		
				bool has_not = static_cast<IfElseNode*>(node)->symbols.has_not;				
				
				compile_node(static_cast<RepeatUntilNode*>(node)->statements, lines, state_number, globalSymbols);
				
				++state_number;										
				
				if (has_not ^  (static_cast<RepeatUntilNode*>(node)->symbols.symbols.find(globalSymbols[0]) == std::string::npos))
				{
					temp += globalSymbols.substr(0,1) + ",q" + std::to_string(old_state_number) + ",@";
				}
				else
				{
					temp += globalSymbols.substr(0,1) + ",q" + std::to_string(state_number ) + ",@";
				}	
				
				for (int i = 1; i < globalSymbols.length(); ++i)
				{
					if (  static_cast<RepeatUntilNode*>(node)->symbols.symbols.find(globalSymbols[i]) == std::string::npos)
					{
						temp += std::string("\t|\t") + globalSymbols.substr(i,1)+ ",q" + std::to_string(old_state_number) + ",@";
					}
					else
					{
						temp += std::string("\t|\t") + globalSymbols.substr(i,1) + ",q" + std::to_string(state_number ) + ",@";
					}	
				}
											
				temp += "\n";							
				lines.push_back(temp);	
				
				static_cast<RepeatUntilNode*>(node)->continue_state = old_state_number ;
				static_cast<RepeatUntilNode*>(node)->break_state = state_number;				

				put_flow_controls(node, lines, state_number, globalSymbols);									
		}	
		break;
										
		default:
						
				throw std::string("\033[31mUnexpected node to compile.\n\033[0m");
		
	}
	

	
}

void Compiler::put_flow_controls(Node* node, std::vector<std::string>& lines, size_t& state_number, const std::string& globalSymbols) &
{
	std::string temp;
	
	for (int i = 0; i < static_cast<LoopNode*>(node)->continue_states.size(); ++i)
	{
		temp = "";
		
		temp += globalSymbols.substr(0,1) + ",q" + std::to_string(static_cast<LoopNode*>(node)->continue_state) + ",@";
		for (int i = 1; i < globalSymbols.length(); ++i)
		{
			temp += std::string("\t|\t") +  globalSymbols.substr(i,1) + ",q" + std::to_string(static_cast<LoopNode*>(node)->continue_state) + ",@";
		}
		temp += "\n";
		lines[static_cast<LoopNode*>(node)->continue_states[i] - 1] = temp;
	}
	
	
	for (int i = 0; i < static_cast<LoopNode*>(node)->break_states.size(); ++i)
	{
		

		temp = "";
		
		temp += globalSymbols.substr(0,1) + ",q" + std::to_string(static_cast<LoopNode*>(node)->break_state) + ",@";
		for (int i = 1; i < globalSymbols.length(); ++i)
		{
			temp += std::string("\t|\t") +  globalSymbols.substr(i,1) + ",q" + std::to_string(static_cast<LoopNode*>(node)->break_state) + ",@";
		}
		temp += "\n";				
		lines[static_cast<LoopNode*>(node)->break_states[i] - 1] = temp;
	}			
	
}
