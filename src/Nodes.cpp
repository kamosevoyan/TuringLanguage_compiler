#include "Nodes.h"

			
Node::Node(TYPE type):type(type)
{
			
}
		
Node::~Node()
{

}

SymbolsNode::SymbolsNode(TYPE type):Node(type)
{
	
}

SymbolsNode::~SymbolsNode()
{

}

PrimaryNode::PrimaryNode(TYPE type):Node(type)
{
	
}

PrimaryNode::~PrimaryNode()
{

}

WriteNode::WriteNode(TYPE type):PrimaryNode(type)
{
	
}

WriteNode::~WriteNode()
{

}

FlowControlNode::FlowControlNode(TYPE type):Node(type)
{
	
}

FlowControlNode::~FlowControlNode()
{

}

StatementNode::StatementNode(TYPE type):Node(type)
{
	
}

StatementNode::~StatementNode()
{
		
}

IfNode::IfNode(TYPE type):StatementNode(type)
{
	
}

IfNode::~IfNode()
{		
	delete this->statements;
}		

IfElseNode::IfElseNode(TYPE type):StatementNode(type)
{
	
}

IfElseNode::~IfElseNode()
{			
	delete this->if_statements;
	delete this->else_statements;
}					

LoopNode::LoopNode(TYPE type):StatementNode(type)
{
	
}

LoopNode::~LoopNode()
{
	delete this->statements;
}				

WhileNode::WhileNode(TYPE type):LoopNode(type)
{
	
}

WhileNode::~WhileNode()
{

}				
		
DoWhileNode::DoWhileNode(TYPE type):LoopNode(type)
{
	
}

DoWhileNode::~DoWhileNode()
{
			
}				
		
RepeatUntilNode::RepeatUntilNode(TYPE type):LoopNode(type)
{
	
}

RepeatUntilNode::~RepeatUntilNode()
{
		
}				

BlockNode::BlockNode(TYPE type):StatementNode(type)
{
	
}

BlockNode::~BlockNode()
{
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
	delete this->statements;
}								


FunctionNode::FunctionNode(TYPE type) : Node(type)
{
	
}

FunctionCallNode::FunctionCallNode(TYPE type, const std::string& name) : Node(type), name(name)
{
	
}