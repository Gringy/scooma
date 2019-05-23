#ifndef __OPCODES_H__
#define __OPCODES_H__

enum vm_opcode {
	// system
	VM_NOP, /*0x00*/
	VM_LIT, /*0x01*/ 
	// arithmetic
	VM_ADD, /*0x02*/ 
	VM_SUB, /*0x03*/
	VM_MUL, /*0x04*/
	VM_DIV, /*0x05*/
	VM_MOD, /*0x06*/
	// logic
	VM_NOT, /*0x07*/
	VM_AND, /*0x08*/
	VM_OR,  /*0x09*/
	VM_XOR, /*0x0A*/
	// comparation
	VM_EQ,  /*0x0B*/
	VM_NEQ, /*0x0C*/
	VM_LT,  /*0x0D*/
	VM_BT,  /*0x0E*/
	VM_LEQ, /*0x0F*/
	VM_BEQ, /*0x10*/
	// stack
	VM_DROP,/*0x11*/
	VM_DUP, /*0x12*/
	VM_NIP, /*0x13*/
	VM_SWAP,/*0x14*/
	VM_ROT, /*0x15*/
	VM_MROT,/*0x16*/
	// rstack
	VM_PUSH,/*0x17*/
	VM_POP, /*0x18*/
	// memory
	VM_FETCH,/*0x19*/
	VM_STORE,/*0x1A*/
	// control flow
	VM_JUMP,/*0x1B*/
	VM_CJUMP,/*0x1C*/
	VM_CALL,/*0x1D*/
	VM_CCALL,/*0x1E*/
	VM_RET, /*0x1F*/
	// io
	VM_DOT, /*0x20*/
	VM_EMIT,/*0x21*/
	VM_GET, /*0x22*/
	VM_DEBUG, /*0x23*/
	// exit
	VM_BYE /*0x24*/
};
#define NUM_OPS (VM_BYE+1)

char *names[] = {
	"VM_NOP",
	"VM_LIT",
	"VM_ADD", 
	"VM_SUB", 
	"VM_MUL", 
	"VM_DIV", 
	"VM_MOD", 
	"VM_NOT", 
	"VM_AND", 
	"VM_OR",  
	"VM_XOR", 
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
	"VM_DOT",
	"VM_EMIT",
	"VM_GET",
	"VM_DEBUG",
	"VM_BYE"
};

#endif