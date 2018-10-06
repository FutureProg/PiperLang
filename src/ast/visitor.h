#if !defined(VISTOR_H)
#define VISTOR_H

#include "ast.h"

// Visitor Flags
enum {
  V_FLAG_NONE = -1,
  V_FLAG_ENTER = 0,
  V_FLAG_EXIT = 1,
  V_FLAG_GUARD = 8,
  V_FLAG_THEN = 0xc,
  V_FLAG_ELSE = 0xe,
};


class Visitor {
	public:
	virtual void visit(NBlock* node, uint64_t flag);	  
  virtual void visit(NBinaryOp* node, uint64_t flag);
  virtual void visit(NIdentifier* node, uint64_t flag);
  virtual void visit(NInteger* node, uint64_t flag);
  virtual void visit(NFloat* node, uint64_t flag);
  virtual void visit(NBool* node, uint64_t flag);
  virtual void visit(NType* node, uint64_t flag);
  virtual void visit(NVariableDeclaration* node, uint64_t flag);
  virtual void visit(NFunction* node, uint64_t flag);
  virtual void visit(NAssignment* node, uint64_t flag);

};

#endif // VISTOR_H
