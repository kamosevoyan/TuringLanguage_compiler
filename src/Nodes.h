#ifndef NODESINCLUDE
#define NODESINCLUDE 

#include <vector>  
#include <string>
#include <iostream>
#include <map>


enum class 
TYPE 
{	
			MAIN, BLOCK, WHILE, DO_WHILE, REPEAT_UNTIL, IF, IF_ELSE, WRITE, 
			LEFT, RIGHT, EXIT, ERROR, CONTINUE, BREAK, SYMBOL_LIST, FUNCTION, FUNCTION_CALL,
			SWITCH, CASE, DEFAULT
};

class Node
{
    friend class Parser;
    friend class Compiler;
    friend void print(Node*, int);	

private:

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

private:		

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

private:

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

private:

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

private:

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

private:

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

private:

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

private:

    std::vector<Node*> statements;
    
public:
        
    BlockNode(TYPE);
    virtual ~BlockNode();					
};

class FunctionNode : public Node
{
	
	friend class Parser;
	friend class Compiler;
	friend void print(Node*, int);		

private:

	Node* statements;
	std::string f_name;
	int entry_state;
	
public:

	FunctionNode(TYPE);
		
};


class FunctionCallNode : public Node
{
	
    friend class Parser;
    friend class Compiler;
    friend void print(Node*, int);		

private:    

    std::string f_name;

public:

    FunctionCallNode(TYPE, const std::string&);		
		
};

class MainNode : public StatementNode 
{
	
    friend class Parser;
    friend class Compiler;
    friend void print(Node*, int);	

private:

    Node* statements;
    
public:
            
    MainNode(TYPE);
    virtual ~MainNode();							
};


class CaseNode : public Node
{
    friend class Parser;
    friend class Compiler;
    friend void print(Node*, int);	

private:

    char symbol;
	Node* statements;
	size_t entry_state;
     
public:
    
    CaseNode(TYPE);
    virtual ~CaseNode();
};	

class DefaultNode : public Node
{
    friend class Parser;
    friend class Compiler;
    friend void print(Node*, int);	

private:

	Node* statements;
    size_t entry_state;
	
public:
    
    DefaultNode(TYPE);
    virtual ~DefaultNode();
};	

class SwitchNode : public Node
{
	friend class Parser;
    friend class Compiler;
    friend void print(Node*, int);	 
 
private:

	std::vector<Node*> cases;
	std::map<char, Node*> branches;
	Node* default_node = nullptr;
	bool has_default = false; 
		
public:	

	SwitchNode(TYPE);
	virtual ~SwitchNode();
};



#endif