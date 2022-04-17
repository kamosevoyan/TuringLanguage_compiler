#include "Compiler.h"

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
	
	
	compile_global_functions(this->parser.globalFunctions, lines, state_number, symbols);
					
	compile_node(node, lines, state_number, symbols);
																			
	print(node);				
	
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
		
		case TYPE::FUNCTION_CALL:
		{
			throw std::string("\033[36mFunctions aren't enable yet. You can only define them.\n\033[0m");
		}			
		break;
		
		default:
		
			throw std::string("\033[31mUnexpected node to compile.\n\033[0m");	
	}	
}


void Compiler::compile_global_functions(std::map<const std::string, FunctionNode*> & function_map, std::vector<std::string>& lines, size_t& state_number, const std::string& globalSymbols) &
{
	std::string temp = "";
	
	for (auto it = function_map.begin(); it != function_map.end(); ++it)
	{
		it->second->entry_state = state_number;		
		compile_node(it->second->statements, lines, state_number, globalSymbols);
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
	if (node->type == TYPE::WRITE)
	{
		std::cout << "write";
	}
	else		
	if (node->type == TYPE::FUNCTION_CALL)
	{
		//std::cout << static_cast<FunctionCallNode*>(node)->name;
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

