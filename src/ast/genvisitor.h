#pragma once
#include <iostream>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <stack>
#include <vector>
#include <map>

#include "visitor.h"

typedef struct STRUCT_SCOPE{
	typedef std::map<std::string ,llvm::Value*> stackitem;
	std::stack<stackitem*> stack;
	stackitem* currentScope() {		
		return stack.top();
	};
	stackitem* open(){
		stackitem* nstack = new stackitem(*stack.top());
		return nstack;
	}
	void close(){
		stack.pop();
	}
	void addVar(std::string varname, llvm::Value* val){
		if(currentScope()->find(varname) != currentScope()->end()){
			printf("Compilation Error: \"%s\" already declared\n",varname.c_str());
			exit(0);
		}
		currentScope()->insert(std::pair<std::string,llvm::Value*>(varname,val));
	}
	llvm::Value* getVar(std::string varname){
		if(currentScope()->find(varname) == currentScope()->end()){
			printf("Compilation Error: \"%s\" not declared\n",varname.c_str());
			exit(0);
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
	llvm::Value* visit(NExpression* expr, uint64_t flag);

	llvm::Type* getTypeFromName(const char* name);	

	private:
	llvm::IRBuilder<>* _builder;
	std::vector<llvm::Module*> _modules;
	llvm::Module* currentModule;
	Scope _scope;
	
};