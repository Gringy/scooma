#ifndef __OPCODES_H__
#define __OPCODES_H__

enum vm_opcode {
	// system
	VM_NOP,
	VM_LIT,
	// arithmetic
	VM_NEG,
	VM_ADD,
	VM_SUB,
	VM_MUL,
	VM_DIV,
	VM_MOD,
	VM_UMUL,
	VM_UDIV,
	VM_UMOD,
	VM_RSH,
	VM_LSH,
	// logic
	VM_NOT,
	VM_AND,
	VM_OR,
	VM_XOR,
	// comparation
	VM_NULL,
	VM_NNULL,
	VM_EQ,
	VM_NEQ,
	VM_LT,
	VM_BT,
	VM_LEQ,
	VM_BEQ,
	// stack
	VM_DROP,
	VM_DUP,
	VM_NIP,
	VM_SWAP,
	VM_OVER,
	VM_ROT,
	VM_MROT,
	// rstack
	VM_PUSH,
	VM_POP,
	// memory
	VM_FETCH,
	VM_STORE,
	// control flow
	VM_JUMP,
	VM_CJUMP,
	VM_CALL,
	VM_CCALL,
	VM_RET,
	// io
	VM_EMIT,
	VM_GET,
	VM_DEBUG,
	// exit
	VM_BYE 
};
#define NUM_OPS (VM_BYE+1)

const char *names[] = {
	"VM_NOP",
	"VM_LIT",
	"VM_NEG",
	"VM_ADD", 
	"VM_SUB", 
	"VM_MUL", 
	"VM_DIV", 
	"VM_MOD", 
	"VM_UMUL",
	"VM_UDIV",
	"VM_UMOD",
	"VM_RSH",
	"VM_LSH",
	"VM_NOT", 
	"VM_AND", 
	"VM_OR",  
	"VM_XOR",
	"VM_NULL",
	"VM_NNULL",
	"VM_EQ",  
	"VM_NEQ", 
	"VM_LT",  
	"VM_BT",  
	"VM_LEQ", 
	"VM_BEQ", 
	"VM_DROP",
	"VM_DUP", 
	"VM_NIP", 
	"VM_SWAP",
	"VM_OVER",
	"VM_ROT", 
	"VM_MROT",
	"VM_PUSH",
	"VM_POP",
	"VM_FETCH",
	"VM_STORE",
	"VM_JUMP",
	"VM_CJUMP",
	"VM_CALL",
	"VM_CCALL",
	"VM_RET",
	"VM_EMIT",
	"VM_GET",
	"VM_DEBUG",
	"VM_BYE"
};

#endif