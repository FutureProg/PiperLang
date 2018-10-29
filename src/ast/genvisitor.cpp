#include "genvisitor.h"

#include <iostream>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

llvm::Function* GenVisitor::visit(NFunction* node, uint64_t flag){	
	std::vector<llvm::Type*> args;
	for(int i = 0; i < node->args.size();i++){
		llvm::Type* type = getTypeFromName(node->args[i]->type->name.c_str());
		args.push_back(type);		
	}
	llvm::FunctionType* retType = llvm::FunctionType::get(_builder->getVoidTy(),false);	
	llvm::Function* func = llvm::Function::Create(retType, llvm::Function::ExternalLinkage, node->name->name,currentModule);	
	visit(node->block,V_FLAG_ENTER);
	return func;
}

llvm::BasicBlock*  GenVisitor::visit(NBlock* node, uint64_t flag){
	llvm::BasicBlock *block = llvm::BasicBlock::Create(_builder->getContext());		
	_builder->SetInsertPoint(block);
	for(int i = 0; i < node->statements.size();i++){	
		NStatement* stmt = node->statements[i];
		switch(stmt->statement_type){
			case STMT_TYPE_BLOCK:
				visit((NBlock*)stmt,V_FLAG_NONE);
				_builder->SetInsertPoint(block);
				break;
			case STMT_TYPE_ASSIGNMENT:
				visit((NAssignment*)stmt,V_FLAG_NONE);
				break;	
			case STMT_TYPE_VAR_DECL:
				visit((NVariableDeclaration*)stmt, V_FLAG_NONE);
				break;
			default:
				printf("Error: unexpected statement type: %d\n",stmt->statement_type);
				break;
		}			
	}	
	return block;
}

void GenVisitor::visit(NVariableDeclaration* node, uint64_t flag){	
	llvm::Type* type = getTypeFromName(node->type->name.c_str());
	std::string name = node->name->name; // TO-DO: deal with checking if it's decl already or not			
	llvm::Value* value = visit(node->expression,V_FLAG_NONE);	

	llvm::AllocaInst *alloc = _builder->CreateAlloca(type);
	_builder->CreateStore(value,alloc);
}

llvm::Value* GenVisitor::visit(NExpression* expr, uint64_t flag){
	switch(expr->expression_type){
		case EXPR_TYPE_BIN_OP:
		return visit((NBinaryOp*)expr,flag);
		case EXPR_TYPE_CONST_BOOL:
		return visit((NBool*)expr,flag);
		case EXPR_TYPE_CONST_INT:
		return visit((NInteger*)expr,flag);
		case EXPR_TYPE_CONST_FLOAT:
		return visit((NFloat*)expr,flag);
	}	 
}

llvm::Value* GenVisitor::visit(NBinaryOp* node, uint64_t flag){
	llvm::Value* lhs = visit(node->lhs,flag);
	llvm::Value* rhs = visit(node->rhs,flag);
	switch(node->op){
		case OP_ADD:
		return _builder->CreateFAdd(lhs,rhs);		
		case OP_SUB:
		return _builder->CreateFSub(lhs,rhs);
		case OP_DIV:
		return _builder->CreateFDiv(lhs,rhs);
		case OP_MULT:
		return _builder->CreateFMul(lhs,rhs);
		default:
		printf("Invalid operator provided to expression\n");
		exit(0);
	}
}

llvm::Value* GenVisitor::visit(NInteger* node, uint64_t flag){
	return llvm::ConstantInt::get(_builder->getInt32Ty(),node->value);
}

llvm::Value* GenVisitor::visit(NFloat* node, uint64_t flag){
	return llvm::ConstantFP::get(_builder->getFloatTy(),node->value);
}

llvm::Value* GenVisitor::visit(NBool* node, uint64_t flag){
	return llvm::ConstantInt::get(_builder->getInt32Ty(),node->value);
}

void GenVisitor::visit(NAssignment* node, uint64_t flag){
	
}

llvm::Type* GenVisitor::getTypeFromName(const char* name){
	if(strcmp(name,"int") == 0){
		return llvm::Type::getInt32Ty(_builder->getContext());
	}
	const llvm::Value* value = _scope.getVar(name);
	return value->getType();
}
