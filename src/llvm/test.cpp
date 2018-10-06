#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>

#include "../ast/genvisitor.h"

using llvm::LLVMContext;
using llvm::Module;
using llvm::IRBuilder;

int main(){
	LLVMContext& context = llvm::getGlobalContext();
	Module* module = new Module("top",context);
	IRBuilder<> builder(context);

	llvm::FunctionType* funcType = llvm::FunctionType::get(builder.getInt32Ty(), false);
	llvm::Function* mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage,"main",module);

	llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
	builder.SetInsertPoint(entry);
	
	llvm::Value *helloworld = builder.CreateGlobalStringPtr("hello world!\n");
	
	std::vector<llvm::Type*> putsArgs;
	putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
	llvm::ArrayRef<llvm::Type*>  argsRef(putsArgs);

	llvm::FunctionType *putsType = llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
	llvm::Constant *putsFunc = module->getOrInsertFunction("puts", putsType);

	llvm::Type *varType = llvm::Type::getInt32Ty(context);
	llvm::Constant *nValue = llvm::ConstantInt::get(builder.getInt32Ty(),4);
	llvm::AllocaInst *alloc =  builder.CreateAlloca(builder.getInt32Ty());	
	builder.CreateStore(nValue,alloc);


	builder.CreateCall(putsFunc, helloworld);

	builder.CreateRetVoid();
	module->dump();
}