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
		throw std::string("Incorrect function definition.\n");
	}
	
	std::string func_name = this->offset_token_value(0);

	if (this->globalFunctions.find(func_name) != this->globalFunctions.end())
	{
		throw std::string("\033[31mThe function " + this->offset_token_value(0) + " is already defined\n\033[0m");
	}
		
	FunctionNode* node = new FunctionNode(TYPE::FUNCTION);
	
	this->next_token();
	
	if (	(this->offset_token(0) != Lexer::KEYWORDS::L_PAR) || (this->offset_token(1) != Lexer::KEYWORDS::R_PAR) )
	{
		throw std::string("\033[31mExpected ()\n\033[0m");
	}
	
	this->next_token();
			
	this->globalFunctions[func_name] = node;
		
	this->next_token();
	
	if (this->offset_token(0) != Lexer::KEYWORDS::L_BR)
	{
		throw std::string("\033[31mfunction statement should only contain block statement.\n\033[0m");
	}
		
	node->statements = parse_statement(node);
	node->name = func_name;	
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
		throw std::string("\033[31mGlobal symbols are not given.\033[0m\n");
	}
	
	SymbolsNode node = parse_symbols_statement();

    if (node.symbols.find("_") == std::string::npos)
    {
        throw std::string("\033[31mError: global symbols should contain empty '_' symbol.\n\033[0m");
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
		throw std::string("\033[31mInvalid syntax " + this->offset_token_value(0) + "\n\033[0m");
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

bool Parser::chek_if_has_exit_states(Node* node) &
{
		
}

Node* Parser::parse_statement(Node* root) &
{	
	Node* node0 = nullptr;
	Node* node1 = nullptr;
	
	if (this->offset_token(0) == Lexer::KEYWORDS::IF)
	{				
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
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::WHILE)
	{
		WhileNode* node = new WhileNode(TYPE::WHILE);
		node->root = root;
		SymbolsNode symnode = parse_symbols_statement();
		
		this->next_token();
		
		node0 = parse_statement(node);
		
		node->statements = node0;
		node->symbols = symnode;
		return node;
	}
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::DO)
	{
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
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::REPEAT)
	{
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
	else
	if (this->offset_token(0)  == Lexer::KEYWORDS::LEFT)
	{					
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
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon\n\033[0m");
		}				
		
		FlowControlNode* node = new FlowControlNode(TYPE::CONTINUE); 
		node->root = root;
		
		if (!	in_loop(node))
		{
			throw std::string("\033[31mFlow control statement continue should be within a loop\n\033[0m");
		}
		
		return node;
	}
	else
	if (this->offset_token(0) == Lexer::KEYWORDS::BREAK)
	{			
		this->next_token();
		
		if (this->offset_token(0)  != Lexer::KEYWORDS::SEMICOLON)
		{
			throw  std::string("\033[31mExpected semicolon\n\033[0m");
		}				
		
		FlowControlNode* node = new FlowControlNode(TYPE::BREAK); 
		node->root = root;
		
		if (!	in_loop(node))
		{
			throw std::string("\033[31mFlow control statement continue should be within a loop\n\033[0m");
		}
		
		return node;					
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
