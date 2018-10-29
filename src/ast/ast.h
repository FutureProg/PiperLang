#
#if !defined(AST_H)
#define AST_H

#include <vector>
#include <string>

using std::string;
using std::vector;

class Node {
	public:
	virtual ~Node(){}
};

enum {
	OP_ADD,
	OP_SUB,
	OP_MULT,
	OP_DIV
};

enum {	
	STMT_TYPE_VAR_DECL,
	STMT_TYPE_ASSIGNMENT,
	STMT_TYPE_BLOCK
};

enum {
	EXPR_TYPE_BIN_OP,
	EXPR_TYPE_CONST_INT,
	EXPR_TYPE_CONST_FLOAT,
	EXPR_TYPE_CONST_BOOL
};

class NStatement: public Node {
	public:
	int statement_type;

	virtual ~NStatement() = default;
};
class NExpression: public Node {
	public:
	int expression_type;
};

class NBlock :public NStatement {
	public:
	vector<NStatement*> statements;
	NBlock(){statement_type = STMT_TYPE_BLOCK;}
};

class NBinaryOp: public NExpression {
	public: 
	int op;
	NExpression* lhs;
	NExpression* rhs;
	NBinaryOp( NExpression* lhs, int op, NExpression* rhs): op(op), lhs(lhs), rhs(rhs){
		expression_type = EXPR_TYPE_BIN_OP;
	}
};

class NAssignment: public NStatement {
	public:
	NExpression* lhs;
	NExpression* rhs;
	NAssignment(NExpression* lhs, NExpression* rhs): lhs(lhs),rhs(rhs){
		statement_type = STMT_TYPE_ASSIGNMENT;		
	}
};

class NIdentifier: public NExpression {
	public:
	string name;
	NIdentifier(string name):name(name){}
};

class NInteger: public NExpression {
	public:
	long long value;
	NInteger(long long value):value(value){
		expression_type = EXPR_TYPE_CONST_INT;
	}
};

class NFloat: public NExpression {
	public:
	float value;
	NFloat(float value):value(value) {
		expression_type = EXPR_TYPE_CONST_FLOAT;
	}
};

class NBool: public NExpression {
	public:
	bool value;
	NBool(bool value):value(value) {
		expression_type = EXPR_TYPE_CONST_BOOL;
	}
};

class NType: public NIdentifier {
	public: 	
	NType( string name):NIdentifier(name){}
};

class NVariableDeclaration: public NStatement {
	public:
	NType* type;
	NIdentifier* name;
	NExpression* expression;
	NVariableDeclaration(NType* type, NIdentifier* name, NExpression* expression):type(type), name(name), expression(expression){
		statement_type = STMT_TYPE_VAR_DECL;
	};
	NVariableDeclaration(NType* type, NIdentifier* name):type(type), name(name){
		statement_type = STMT_TYPE_VAR_DECL;
	};
};

class NFunction: public NBlock {
	public:
	vector<NIdentifier*> sources;
	NIdentifier* name;
	vector<NVariableDeclaration*> args;
	vector<NType*> type;
	NBlock* block;

	NFunction(vector<NIdentifier*>* sources, NIdentifier* name,vector<NVariableDeclaration*>* args,
			vector<NType*>* type, NBlock* block):
			sources(*sources), name(name), args(*args), type(*type), block(block){}
};


#endif // AST_H
