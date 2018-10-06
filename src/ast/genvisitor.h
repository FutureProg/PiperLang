#pragma once
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <stack>
#include <vector>
#include <map>

#include "visitor.h"

typedef struct {
	typedef std::map<std::string ,llvm::Value*> stackitem;
	std::stack<stackitem*> stack;
	stackitem* currentScope() {		
		return stack.top();
	};
	stackitem* open(){
		stackitem* nstack = new stackitem(*stack.top());
	}
	void close(){
		stack.pop();
	}
	void addVar(std::string varname, llvm::Value* val){
		if(currentScope()->find(varname) != currentScope()->end()){
			throw "Compilation Error: \"" + varname + "\" already declared";
		}
		currentScope()->insert_or_assign(varname, val);
	}
	llvm::Value* getVar(std::string varname){
		if(currentScope()->find(varname) == currentScope()->end()){
			throw "Compilation Error: \"" + varname + "\" not declared";
		}
		return currentScope()->at(varname);
	}

} Scope;

class GenVisitor {

	public:

	GenVisitor(llvm::IRBuilder<>* builder):_builder(builder){};

	llvm::BasicBlock* visit(NBlock* node, uint64_t flag);	  
	llvm::Value* visit(NBinaryOp* node, uint64_t flag);
	void visit(NIdentifier* node, uint64_t flag);
	llvm::Value* visit(NInteger* node, uint64_t flag);
	llvm::Value* visit(NFloat* node, uint64_t flag);
	llvm::Value* visit(NBool* node, uint64_t flag);
	void visit(NType* node, uint64_t flag);
	void visit(NVariableDeclaration* node, uint64_t flag);
	llvm::Function* visit(NFunction* node, uint64_t flag);
	void visit(NAssignment* node, uint64_t flag);
	llvm::Value* GenVisitor::visit(NExpression* expr, uint64_t flag);

	llvm::Type* getTypeFromName(const char* name);	

	private:
	llvm::IRBuilder<>* _builder;
	std::vector<llvm::Module*> _modules;
	llvm::Module* currentModule;
	Scope _scope;
	
};