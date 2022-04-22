#include "Parser.h"

Parser::Parser()
{
	
}

void Parser::next_token()&
{
	this->token_count += 1;
}
		
Lexer::KEYWORDS Parser::offset_token(int offset) &
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
			
FunctionNode* Parser::parse_function_statement() &
{
	if (this->offset_token(0) != Lexer::KEYWORDS::NAME)
	{
		throw std::string("Incorrect function definition.");
	}
	
	std::string func_name = this->offset_token_value(0);

	if (this->globalFunctions.find(func_name) != this->globalFunctions.end())
	{
		throw std::string("Function " + this->offset_token_value(0) + " is already defined.");
	}
		
	FunctionNode* node = new FunctionNode(TYPE::FUNCTION);
	
	this->next_token();
	
	if (	(this->offset_token(0) != Lexer::KEYWORDS::L_PAR) || (this->offset_token(1) != Lexer::KEYWORDS::R_PAR) )
	{
		throw std::string("Expected ().");
	}
	
	this->next_token();
			
	this->globalFunctions[func_name] = node;
		
	this->next_token();
	
	if (this->offset_token(0) != Lexer::KEYWORDS::L_BR)
	{
		throw std::string("Function statement should only contain block statement.");
	}
		
	node->statements = parse_statement(node);
	node->f_name = func_name;	
	return node;
}

void Parser::parse_all_function_statements() &
{
	while (this->offset_token(0) == Lexer::KEYWORDS::FUNCTION)
	{
			this->next_token();
			FunctionNode* func = parse_function_statement();
			//this->globalFunctions[func->name] = func;
			this->next_token();
	}
}

void Parser::get_global_symbols() &
{
	this->next_token();
	if (this->offset_token(0) != Lexer::KEYWORDS::GLOBAL_SYMBOLS)
	{
		throw std::string("Global symbols are not given.");
	}
	
	SymbolsNode node = parse_symbols_statement();

    if (node.symbols.find("_") == std::string::npos)
    {
        throw std::string("Error: global symbols should contain empty '_' symbol.");
    }
	
	this->globalSymbols = node;
}

MainNode* Parser::parse() &
{
	this->get_global_symbols();	
	this->next_token();
	
	parse_all_function_statements();

	MainNode* node = parse_main_statement();
	node->root = nullptr;	
	this->next_token();
	
	if (this->offset_token(0) != Lexer::KEYWORDS::END)
	{
		throw std::string("Invalid syntax.");
	}
	
	return node;
}
		
MainNode* Parser::parse_main_statement() &
{
		
	if (this->offset_token(0) != Lexer::KEYWORDS::MAIN)
	{
		throw std::string("\033[31mExpected statement\n\033[0m");
	}
	
	
	this->next_token();
	
	if (	(this->offset_token(0) != Lexer::KEYWORDS::L_PAR) || (this->offset_token(1) != Lexer::KEYWORDS::R_PAR) )
	{
		throw std::string("\033[31mIncorrect statement\n\033[0m");
	}
	
	this->next_token();
	MainNode* node = new MainNode(TYPE::MAIN);
	node->symbols = this->globalSymbols;
	this->next_token();
	
	if (this->offset_token(0) != Lexer::KEYWORDS::L_BR)
	{
		throw std::string("\033[31mMain statement should only contain block statement.\n\033[0m");
	}
	
	node->statements = parse_statement(node);					
	return node;
}

Node* Parser::parse_statement(Node* root) &
{	
	Node* node0 = nullptr;
	Node* node1 = nullptr;
	
	if (this->offset_token(0) == Lexer::KEYWORDS::IF)
	{				
		return parse_if_statement(root);
	}		
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::WHILE)
	{
		return parse_while_statement(root);
	}
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::DO)
	{
		return parse_do_while_statement(root);
	}
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::REPEAT)
	{
		return parse_repeat_until_statement(root);
	}			
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::L_BR)
	{
		return parse_block_statement(root);
	}
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::LEFT)
	{					
		return parse_left_statement(root);
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::RIGHT)
	{			
		return parse_right_statement(root);
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::EXIT)
	{				
		return parse_exit_statement(root);
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::ERROR)
	{			
		return parse_error_statement(root);
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::WRITE)
	{		
		return parse_write_statement(root);
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::CONTINUE)
	{			
		return parse_continue_statement(root);
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::BREAK)
	{			
		return parse_break_statement(root);
	}
	else 
	if (this->offset_token(0) == Lexer::KEYWORDS::SWITCH)
	{
		return parse_switch_statement(root);
	}
	else 
	if (this->offset_token(0) == Lexer::KEYWORDS::DEFAULT)
	{
		throw std::string("Default statement should only be in switch statement.");
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::CASE)
	{
		throw std::string("Case statement should only be in switch statement.");
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::NAME)
	{
		//Threat as function call
		
		std::string name = this->offset_token_value(0) ;
		this->next_token();
		
		if (	(this->offset_token(0) != Lexer::KEYWORDS::L_PAR) || (this->offset_token(1) != Lexer::KEYWORDS::R_PAR) )
		{
			throw std::string("\033[31mIncorrect statement\n\033[0m");
		}
		
		this->next_token();
		this->next_token();
		
		if (this->globalFunctions.find(name) == this->globalFunctions.end())
		{
			throw std::string("\033[31mThe function " + name + " is not defined \n\033[0m");
		}
		
		//needs some correcting.
		
		return new FunctionCallNode(TYPE::FUNCTION_CALL, name);
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
		throw std::string("\Else statement should follow by an if statement.");
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::END)
	{
		throw std::string("Expected statement.");
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::MAIN)
	{
		throw  std::string("Main statement should occur only once.");
	}
	else 
	{						
		throw std::string("Unknow statement " + this->offset_token_value(0) + ".");
	}			

}

Node* Parser::parse_if_statement(Node* root)&
{
	Node* node0 = nullptr;
	Node* node1 = nullptr;
	
	IfNode* first_node = new IfNode(TYPE::IF);
	first_node->root = root;				
	SymbolsNode symnode = parse_symbols_statement();
	
	this->next_token();				
	node0 = parse_statement(first_node);
	
	if (this->offset_token(1) == Lexer::KEYWORDS::ELSE)
	{					
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
		return node;
	}
									
	first_node->statements = node0;
	first_node->symbols = symnode;										
	return first_node;
}

Node* Parser::parse_while_statement(Node* root)&
{
	Node* node0 = nullptr;
	
	WhileNode* node = new WhileNode(TYPE::WHILE);
	node->root = root;
	SymbolsNode symnode = parse_symbols_statement();
	
	this->next_token();
	
	node0 = parse_statement(node);
	
	node->statements = node0;
	node->symbols = symnode;
	return node;
}

Node* Parser::parse_do_while_statement(Node* root)&
{
	Node* node0 = nullptr;
	DoWhileNode* node = new DoWhileNode(TYPE::DO_WHILE);
	node->root = root;
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
	return node;	
}

Node* Parser::parse_repeat_until_statement(Node* root)&
{
	Node* node0 = nullptr;
	
	RepeatUntilNode* node = new RepeatUntilNode(TYPE::REPEAT_UNTIL);
	node->root = root;
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
	return node;	
}

Node* Parser::parse_block_statement(Node* root)&
{
	Node* node0 = nullptr;
	
	BlockNode* node = new BlockNode(TYPE::BLOCK);
	node->root = root;
	
	this->next_token();

	if ( this->offset_token(0) == Lexer::KEYWORDS::R_BR)
	{
		throw std::string("\033[31mEmpty block statement\n\033[0m");
	}

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
	return node;	
}

Node* Parser::parse_left_statement(Node* root)&
{
	this->next_token();
	
	if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
	{
		throw  std::string("\033[31mExpected semicolon\n\033[0m");
	}				
	
	return new PrimaryNode(TYPE::LEFT);
}

Node* Parser::parse_right_statement(Node* root)&
{
	this->next_token();
	
	if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
	{
		throw  std::string("\033[31mExpected semicolon\n\033[0m");
	}				
	
	return new PrimaryNode(TYPE::RIGHT);
}

Node* Parser::parse_exit_statement(Node* root)&
{
	this->next_token();
	
	if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
	{
		throw  std::string("\033[31mExpected semicolon\n\033[0m");
	}				
	
	return new PrimaryNode(TYPE::EXIT);	
}

Node* Parser::parse_error_statement(Node* root)&
{
	this->next_token();
	
	if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
	{
		throw  std::string("\033[31mExpected semicolon\n\033[0m");
	}				
	
	return new PrimaryNode(TYPE::ERROR);
}

Node* Parser::parse_write_statement(Node* root)&
{
	SymbolsNode symnode = parse_symbols_statement();
	 
	if (symnode.symbols.length() > 1)
	{
		throw std::string("\033[31mExpected only one symbol in write statement\n\033[0m");
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

Node* Parser::parse_continue_statement(Node* root)&
{
	this->next_token();
	
	if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
	{
		throw  std::string("\033[31mExpected semicolon\n\033[0m");
	}				
	
	FlowControlNode* node = new FlowControlNode(TYPE::CONTINUE); 
	node->root = root;
	
	if (!	in_loop(node))
	{
		throw std::string("\033[31mFlow control statement continue should only be within a loop\n\033[0m");
	}
	
	return node;	
}

Node* Parser::parse_break_statement(Node* root)&
{
	this->next_token();
	
	if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
	{
		throw  std::string("Expected semicolon.");
	}				
	
	FlowControlNode* node = new FlowControlNode(TYPE::BREAK); 
	node->root = root;
	
	if (!	in_loop(node))
	{
		throw std::string("Flow control statement continue should only be within a loop.");
	} 
	
	return node;		
}

SwitchNode* Parser::parse_switch_statement(Node* root) &
{
		Node* node0 = nullptr;
		
		this->next_token();
		
		if ((this->offset_token(0) != Lexer::KEYWORDS::L_PAR) || (this->offset_token(1) != Lexer::KEYWORDS::R_PAR))
		{
			throw std::string("Incorrect statement.");
		}
		
		this->next_token();
		this->next_token();
	
		if (this->offset_token(0) != Lexer::KEYWORDS::L_BR)
		{
			std::cout << "wrong type = " << (int)this->offset_token(0) << "\n";
			throw std::string("Switch statement should only contain block statement.");
		}
		
		this->next_token();
		
		SwitchNode* node = new SwitchNode(TYPE::SWITCH);
		node->root = root;
				
		while (this->offset_token(0)  != Lexer::KEYWORDS::R_BR)
		{
			
			if ((this->offset_token(0) == Lexer::KEYWORDS::CASE) || (this->offset_token(0) == Lexer::KEYWORDS::DEFAULT))
			{	
				node0 = parse_case_and_default_statement(node);
			}
			else
			{
				throw std::string("Only case statements and default statement are alloved in switch statement.");
			}
			  
			if (node->branches.find(static_cast<CaseNode*>(node0)->symbol) != node->branches.end())
			{
				throw std::string("Duplicate case value.");
			}			
			else
			{
				node->branches[static_cast<CaseNode*>(node0)->symbol] = node0;
			}
			
			if (node0->type == TYPE::DEFAULT)
			{
				if (node->has_default)
				{
					throw std::string("Multiple default labels in one switch statement.");
				}
				else
				{
					node->has_default = true;
					node->default_node = node0; 
				}
			}
			
			node->cases.insert(node->cases.end(), node0);

			this->next_token();
		} 
		
		return node;
}

Node* Parser::parse_case_and_default_statement(Node* root)&
{
		Node* node0 = nullptr;
		
		if (this->offset_token(0) == Lexer::KEYWORDS::DEFAULT)
		{
			return parse_default_statement(root);
		}
		
		SymbolsNode symnode = parse_symbols_statement();
		
		if (symnode.symbols.length() > 1)
		{
			throw std::string("\033[31mExpected only one symbol in case statement\n\033[0m");
		}
		
		this->next_token();
		
		if (this->offset_token(0) != Lexer::KEYWORDS::COLON)
		{
			throw std::string("\033[31mExpected colon after case statement.\n\033[0m");
		}
 		
		CaseNode* node= new CaseNode(TYPE::CASE);
		node->root = root;
			
		node->symbol = symnode.symbols[0];
		
		this->next_token();
		node0 = parse_statement(node);
		node->statements = node0;
		
		return node;
}

DefaultNode* Parser::parse_default_statement(Node* root)&
{
	Node*node0 = nullptr;
	this->next_token();
	
	if (this->offset_token(0) != Lexer::KEYWORDS::COLON)
	{
		throw std::string("\033[31mExpected colon after default statement.\n\033[0m");
	}
	
	this->next_token();
	
	DefaultNode* node= new DefaultNode(TYPE::DEFAULT);
	node->root = root;
				
	node0 = parse_statement(node);
	node->statements = node0;
	
	return node;
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
		
	if (this->offset_token(1) == Lexer::KEYWORDS::NOT)
	{
		this->next_token();
		
		if (isGlobal)
		{
			throw std::string("Main statement should not have 'not' attribute.");
		}
		has_not = true;
	}
	else
	{
		has_not = false;
	}
	
	this->next_token();
	
	if (this->offset_token(0)  != Lexer::KEYWORDS::L_PAR)
	{		
		throw std::string("Expected left parenrhesis.");
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
				throw std::string("Empty parenthesis statement.");
			}
			break;
		}
		
		if ((this->offset_token(0)  != Lexer::KEYWORDS::SYMBOL) && ( ! first_was_symbol))
		{
			throw std::string("Expected symbol before comma.");
		}
		
		first_was_symbol = true;
		
		if (this->offset_token(0)  == Lexer::KEYWORDS::SYMBOL)
		{
			
			if (std::find(node.symbols.begin(), node.symbols.end(), this->offset_token_value(0)[0]) != node.symbols.end())
			{
				
				throw std::string("Symbol " + this->offset_token_value(0) + " is already given.");
			}
			
			if (!isGlobal)
			{
				if (std::find(this->globalSymbols.symbols.begin(), this->globalSymbols.symbols.end(), this->offset_token_value(0)[0]) == this->globalSymbols.symbols.end())
				{
					throw std::string("The symbol " + this->offset_token_value(0) + " is not given in main symbols.");
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
			throw std::string("Expected symbol.");
		}
		
	}
	
		if (las_was_comma)
		{
			throw std::string("Expected symbol after comma.");
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
