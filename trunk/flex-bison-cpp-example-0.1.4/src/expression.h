// $Id: expression.h 48 2009-09-05 08:07:10Z tb $
/** \file expression.h Implements an example calculator class group. */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <map>
#include <vector>
#include <ostream>
#include <stdexcept>
#include <cmath>
#include <stdlib.h>

enum treetype {statement_node, number_node, variable_node};
enum stmttype {select_st, insert_st, delete_st, drop_st};
typedef struct tree_t {
	enum treetype nodetype;
	union {
		struct {
			struct tree_t *arg1, *arg2, *arg3, *arg4, *arg5; 
			enum stmttype type;
		} stmt;
		int number;
   	    char* variable;
	} body;
} tree;

class SQLTree{
private:
public:
    int count;

    SQLTree(){
    }

	//void make_stmt (tree *arg1, tree *arg2, tree *arg3, tree *arg4, tree *arg5, enum stmttype t) {
        //printf(arg1->body.variable);

		//root = new(tree);
		//root->nodetype= statement_node;
		//root->body.stmt.arg1= arg1;
		//root->body.stmt.arg2= arg2;
		//root->body.stmt.arg3= arg3;
		//root->body.stmt.arg4= arg4;
		//root->body.stmt.arg5= arg5;
		//root->body.stmt.arg5= arg5;
		//root->body.stmt.type= t;
	//}

	tree* make_stmt (tree *arg1, tree *arg2, tree *arg3, tree *arg4, tree *arg5, enum stmttype t) {
		tree* result = new(tree);
		result->nodetype= statement_node;
		result->body.stmt.arg1= arg1;
		result->body.stmt.arg2= arg2;
		result->body.stmt.arg3= arg3;
		result->body.stmt.arg4= arg4;
		result->body.stmt.arg5= arg5;
		result->body.stmt.arg5= arg5;
		result->body.stmt.type= t;

		return result;
	}

	tree* make_variable (std::string v) {
		tree* result = new(tree);
		result->nodetype= variable_node;

		size_t length = v.length();
		if(length== 0){
			printf("string should not be null\n");
			result->body.variable="";
		}else{
			char* pBuf = new char[length+1];
			v.copy(pBuf,length,0);
			pBuf[length]='\0';
			result->body.variable= pBuf;
		} 

		return result;
	}

	void print_tree (tree *t,int level) {
printf("here %d",count);
		//int step =4;
		//if (t!= NULL)
		//	switch (t->nodetype)
		//{
		//	case statement_node:
		//		if(t->body.stmt.type==drop_st){
		//			print_tree (t->body.stmt.arg1, level+step);
		//			printf("DROP TABLE\n");
		//			break;
		//		}
		//	case variable_node:
		//		printf ("%*c%*s\n", level, ' ', t->body.variable);
		//		printf("%*c%s\n", level, ' ', t->body.variable);
		//		break;
		//}
	}

	//tree* getTree(){
    //return this->root;
	//}

};





/** CalcNode is the abstract base class for calculation nodes. From it the
 * different nullary, unary and binary nodes are derived. */
class CalcNode
{
public:
    /// required for virtual functions. in the derived classes the operands are
    /// deleted.
    virtual ~CalcNode()
    {
    }

    /// evaluate the complete calculation tree and return the floating point
    /// result value
    virtual double	evaluate() const = 0;

    /// output the calculation tree to the given stream. tries to format the
    /// output to make tree levels visible.
    virtual void	print(std::ostream &os, unsigned int depth=0) const = 0;

    /// helper function for print() which makes the indention string
    static inline std::string indent(unsigned int d)
    {
	return std::string(d * 2, ' ');
    }
};

/** Calculation node always returning a constant value. */
class CNConstant : public CalcNode
{
    /// the constant value returned
    double	value;
    
public:
    /// construct a constant calculation node from a value
    explicit CNConstant(double _value)
	: CalcNode(), value(_value)
    {
    }

    virtual double evaluate() const
    {
	return value;
    }

    virtual void print(std::ostream &os, unsigned int depth) const
    {
	os << indent(depth) << value << std::endl;
    }
};

/** Calculation node negating the value of the operand subtree. */
class CNNegate : public CalcNode
{
    /// calculation subtree
    CalcNode* 	node;

public:
    explicit CNNegate(CalcNode* _node)
	: CalcNode(), node(_node)
    {
    }

    virtual ~CNNegate()
    {
	delete node;
    }

    virtual double evaluate() const
    {
	return - node->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const
    {
	os << indent(depth) << "- negate" << std::endl;
	node->print(os, depth+1);
    }
};

/** Calculation node adding two operand nodes. */
class CNAdd : public CalcNode
{
    /// left calculation operand
    CalcNode* 	left;

    /// right calculation operand
    CalcNode* 	right;
    
public:
    explicit CNAdd(CalcNode* _left, CalcNode* _right)
	: CalcNode(), left(_left), right(_right)
    {
    }

    virtual ~CNAdd()
    {
	delete left;
	delete right;
    }

    virtual double evaluate() const
    {
	return left->evaluate() + right->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const
    {
	os << indent(depth) << "+ add" << std::endl;
	left->print(os, depth+1);
	right->print(os, depth+1);
    }
};

/** Calculation node subtracting two operand nodes. */
class CNSubtract : public CalcNode
{
    /// left calculation operand
    CalcNode* 	left;

    /// right calculation operand
    CalcNode* 	right;
    
public:
    explicit CNSubtract(CalcNode* _left, CalcNode* _right)
	: CalcNode(), left(_left), right(_right)
    {
    }

    virtual ~CNSubtract()
    {
	delete left;
	delete right;
    }

    virtual double evaluate() const
    {
	return left->evaluate() - right->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const
    {
	os << indent(depth) << "- subtract" << std::endl;
	left->print(os, depth+1);
	right->print(os, depth+1);
    }
};

/** Calculation node multiplying two operand nodes. */
class CNMultiply : public CalcNode
{
    /// left calculation operand
    CalcNode* 	left;

    /// right calculation operand
    CalcNode* 	right;
    
public:
    explicit CNMultiply(CalcNode* _left, CalcNode* _right)
	: CalcNode(), left(_left), right(_right)
    {
    }

    virtual ~CNMultiply()
    {
	delete left;
	delete right;
    }

    virtual double evaluate() const
    {
	return left->evaluate() * right->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const
    {
	os << indent(depth) << "* multiply" << std::endl;
	left->print(os, depth+1);
	right->print(os, depth+1);
    }
};

/** Calculation node dividing two operand nodes. */
class CNDivide : public CalcNode
{
    /// left calculation operand
    CalcNode* 	left;

    /// right calculation operand
    CalcNode* 	right;
    
public:
    explicit CNDivide(CalcNode* _left, CalcNode* _right)
	: CalcNode(), left(_left), right(_right)
    {
    }

    virtual ~CNDivide()
    {
	delete left;
	delete right;
    }

    virtual double evaluate() const
    {
	return left->evaluate() / right->evaluate();
    }

    virtual void print(std::ostream &os, unsigned int depth) const
    {
	os << indent(depth) << "/ divide" << std::endl;
	left->print(os, depth+1);
	right->print(os, depth+1);
    }
};

/** Calculation node calculating the remainder of an integer division of two
 * operand nodes. */
class CNModulo : public CalcNode
{
    /// left calculation operand
    CalcNode* 	left;

    /// right calculation operand
    CalcNode* 	right;
    
public:
    explicit CNModulo(CalcNode* _left, CalcNode* _right)
	: CalcNode(), left(_left), right(_right)
    {
    }

    virtual ~CNModulo()
    {
	delete left;
	delete right;
    }

    virtual double evaluate() const
    {
	return std::fmod(left->evaluate(), right->evaluate());
    }

    virtual void print(std::ostream &os, unsigned int depth) const
    {
	os << indent(depth) << "% modulo" << std::endl;
	left->print(os, depth+1);
	right->print(os, depth+1);
    }
};

/** Calculation node raising one operand to the power of the second. */
class CNPower : public CalcNode
{
    /// left calculation operand
    CalcNode* 	left;

    /// right calculation operand
    CalcNode* 	right;
    
public:
    explicit CNPower(CalcNode* _left, CalcNode* _right)
	: CalcNode(), left(_left), right(_right)
    {
    }

    virtual ~CNPower()
    {
	delete left;
	delete right;
    }

    virtual double evaluate() const
    {
	return std::pow(left->evaluate(), right->evaluate());
    }

    virtual void print(std::ostream &os, unsigned int depth) const
    {
	os << indent(depth) << "^ power" << std::endl;
	left->print(os, depth+1);
	right->print(os, depth+1);
    }
};

/** Calculator context used to save the parsed expressions. This context is
 * passed along to the example::Driver class and fill during parsing via bison
 * actions. */
class CalcContext
{
public:

	std::vector<tree*> stmt_vector;
	SQLTree* aSQLTree;

    /// type of the variable storage
    typedef std::map<std::string, double> variablemap_type;
    /// variable storage. maps variable string to doubles
    variablemap_type		variables;
    /// array of unassigned expressions found by the parser. these are then
    /// outputted to the user.
    std::vector<CalcNode*>	expressions;

    /// free the saved expression trees
    ~CalcContext()
    {
	clearExpressions();
    }

    /// free all saved expression trees
    void	clearExpressions()
    {
	for(unsigned int i = 0; i < expressions.size(); ++i)
	{
	    delete expressions[i];
	}
	expressions.clear();
    }

    /// check if the given variable name exists in the storage
    bool	existsVariable(const std::string &varname) const
    {
	return variables.find(varname) != variables.end();
    }
    
    /// return the given variable from the storage. throws an exception if it
    /// does not exist.
    double	getVariable(const std::string &varname) const
    {
	variablemap_type::const_iterator vi = variables.find(varname);
	if (vi == variables.end())
	    throw(std::runtime_error("Unknown variable."));
	else
	    return vi->second;
    }
};

#endif // EXPRESSION_H
