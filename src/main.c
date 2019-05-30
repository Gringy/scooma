#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "opcodes.h"

#define CELL int32_t
#define UCELL uint32_t

// MiB
#define MEMORY_SIZE 256*1024
#define STACK_SIZE 1024
#define RSTACK_SIZE 1024

CELL ip, sp, rp;
CELL stack[STACK_SIZE];
CELL rstack[RSTACK_SIZE];
CELL memory[MEMORY_SIZE];

const char *filename_parameter = "image.bin";
int debug_flag;

int vm_load(const char *filename) {
	FILE *image;
	if ((image = fopen(filename, "rb")) == NULL)
		return 0;
	fseek(image, 0, SEEK_END);
	long fileLen = ftell(image) / sizeof(CELL);
	rewind(image);
	long imageSize = fread(&memory, sizeof(CELL), fileLen, image);
	printf("read image of %ld cells\n", imageSize);
	return -1;
}

typedef void(*Op)(void);

void inst_nop() {}

void inst_lit() {
	stack[sp++] = memory[++ip];
}
void inst_neg() {
	stack[sp-1] = - stack[sp-1];
}
void inst_add() {
	CELL top = stack[--sp];
	stack[sp-1] += top;
}
void inst_sub() {
	CELL top = stack[--sp];
	stack[sp-1] -= top;
}
void inst_mul() {
	CELL top = stack[--sp];
	stack[sp-1] *= top;
}
void inst_div() {
	CELL top = stack[--sp];
	stack[sp-1] /= top;
}
void inst_mod() {
	CELL top = stack[--sp];
	stack[sp-1] %= top;
}
void inst_umul() {
	UCELL top = stack[--sp];
	stack[sp-1] = ((UCELL)stack[sp-1]) * top;
}
void inst_udiv() {
	UCELL top = stack[--sp];
	stack[sp-1] = ((UCELL)stack[sp-1]) / top;
}
void inst_umod() {
	UCELL top = stack[--sp];
	stack[sp-1] = ((UCELL)stack[sp-1]) % top;
}
void inst_rsh() {
	UCELL top = stack[--sp];
	stack[sp-1] = ((UCELL)stack[sp-1]) >> top;
}
void inst_lsh() {
	CELL top = stack[--sp];
	stack[sp-1] <<= top;
}
void inst_not() {
	stack[sp-1] = ~stack[sp-1];
}
void inst_and() {
	CELL top = stack[--sp];
	stack[sp-1] &= top;
}
void inst_or() {
	CELL top = stack[--sp];
	stack[sp-1] |= top;
}
void inst_xor() {
	CELL top = stack[--sp];
	stack[sp-1] ^= top;
}
void inst_null() {
	stack[sp-1] = stack[sp-1] == 0 ? -1 : 0;
}
void inst_nnull() {
	stack[sp-1] = stack[sp-1] != 0 ? -1 : 0;
}
void inst_eq() {
	CELL top = stack[--sp];
	if (stack[sp-1] == top)
		stack[sp-1] = -1;
	else
		stack[sp-1] = 0;
}
void inst_neq() {
	CELL top = stack[--sp];
	if (stack[sp-1] != top)
		stack[sp-1] = -1;
	else
		stack[sp-1] = 0;
}
void inst_lt() {
	CELL top = stack[--sp];
	if (stack[sp-1] < top)
		stack[sp-1] = -1;
	else
		stack[sp-1] = 0;
}
void inst_bt() {
	CELL top = stack[--sp];
	if (stack[sp-1] > top)
		stack[sp-1] = -1;
	else
		stack[sp-1] = 0;
}
void inst_leq() {
	CELL top = stack[--sp];
	if (stack[sp-1] <= top)
		stack[sp-1] = -1;
	else
		stack[sp-1] = 0;
}
void inst_beq() {
	CELL top = stack[--sp];
	if (stack[sp-1] >= top)
		stack[sp-1] = -1;
	else
		stack[sp-1] = 0;
}
void inst_drop() {
	sp--;
}
void inst_dup() {
	CELL top = stack[sp-1];
	stack[sp++] = top;
}
void inst_nip() {
	stack[sp-2] = stack[sp-1];
	sp--;
}
void inst_swap() {
	CELL top = stack[sp-1];
	CELL under = stack[sp-2];
	stack[sp-1] = under;
	stack[sp-2] = top;
}
void inst_over() {
	CELL under = stack[sp-2];
	stack[sp++] = under;
}
void inst_rot() {
	// a b c -- b c a
	CELL top1 = stack[sp-1];
	CELL top2 = stack[sp-2];
	CELL top3 = stack[sp-3];
	stack[sp-1] = top3;
	stack[sp-2] = top1;
	stack[sp-3] = top2;
}
void inst_mrot() {
	// a b c -- c a b
	CELL top1 = stack[sp-1];
	CELL top2 = stack[sp-2];
	CELL top3 = stack[sp-3];
	stack[sp-1] = top2;
	stack[sp-2] = top3;
	stack[sp-3] = top1;
}
void inst_push() {
	rstack[rp++] = stack[--sp];
}
void inst_pop() {
	stack[sp++] = rstack[--rp];
}
void inst_fetch() {
	// a -- n
	CELL address = stack[sp-1];
	stack[sp-1] = memory[address];
}
void inst_store() {
	// n a --
	CELL address = stack[--sp];
	CELL n = stack[--sp];
	memory[address] = n;
}
void inst_jump() {
	CELL address = stack[--sp];
	// address-1 to fix ip++ in main execution function
	ip = address - 1;
}
void inst_cjump() {
	CELL address = stack[--sp];
	CELL bool = stack[--sp];
	if (bool)
		ip = address - 1;
}
void inst_call() {
	CELL address = stack[--sp];
	rstack[rp++] = ip;
	ip = address - 1;
}
void inst_ccall() {
	CELL address = stack[--sp];
	CELL bool = stack[--sp];

	if (bool) {
		rstack[rp++] = ip;
		ip = address - 1;
	}
}
void inst_ret() {
	ip = rstack[--rp];
}
void inst_emit() {
	printf("%c", stack[--sp]);
}
void inst_get() {
	int ch = getchar();
	stack[sp++] = ch;
}
void inst_debug() {
	printf("==============================\n");
	printf("ip:%d\n", ip);

	printf("stack %d: [", sp);
	for (int i = 0; i < sp; i++)
		printf("%d ", stack[i]);
	printf("]\n");

	printf("rstack: [");
	for (int i = 0; i < rp; i++)
		printf("%d ", rstack[i]);
	printf("]\n");

	printf("buffer: [");
	for (int i = 4096; i < 4120; i++)
		printf("%d ", memory[i]);
	printf("]\n");
}
void inst_bye() {
	ip = MEMORY_SIZE;
	printf("bye!\n");
}

Op instructions[NUM_OPS];

void vm_print_stack() {
	printf("[");
	for (int i = 0; i < sp-1; i++)
		printf("%d,", stack[i]);
	if (sp >= 1)
		printf("%d", stack[sp-1]);
	printf("]");
}

void perform(uint8_t opcode) {
	if (opcode >= NUM_OPS) {
		printf("unknown opcode #%x\n", opcode);
		exit(1);
	}
	if (debug_flag && opcode != 0) {
		printf("run opcode #%02x(%s) ", opcode, names[opcode]);
		vm_print_stack();
		printf("\n");
	}
	instructions[opcode]();
}

void vm_run() {
	while (ip >= 0 && ip < MEMORY_SIZE) {
		if (debug_flag) {
			printf("### next op on 0x%04x ", ip);
			vm_print_stack();
			printf("\n");
		}
		uint32_t opcode = memory[ip];
		perform((opcode)       & 0xFF);
		perform((opcode >> 8)  & 0xFF);
		perform((opcode >> 16) & 0xFF);
		perform((opcode >> 24) & 0xFF);
		ip++;
	}
}

void vm_prepare() {
	ip = sp = rp = 0;
	
	for (int i = 0; i < STACK_SIZE; i++)
		stack[i] = 0;
	for (int i = 0; i < RSTACK_SIZE; i++)
		rstack[i] = 0;	
	for (int i = 0; i < MEMORY_SIZE; i++)
		memory[i] = VM_NOP;

	instructions[VM_NOP] = inst_nop;
	instructions[VM_LIT] = inst_lit;

	instructions[VM_NEG] = inst_neg;
	instructions[VM_ADD] = inst_add;
	instructions[VM_SUB] = inst_sub;
	instructions[VM_MUL] = inst_mul;
	instructions[VM_DIV] = inst_div;
	instructions[VM_MOD] = inst_mod;
	instructions[VM_UMUL] = inst_umul;
	instructions[VM_UDIV] = inst_udiv;
	instructions[VM_UMOD] = inst_umod;
	instructions[VM_RSH] = inst_rsh;
	instructions[VM_LSH] = inst_lsh;

	instructions[VM_NOT] = inst_not;
	instructions[VM_AND] = inst_and;
	instructions[VM_OR ] = inst_or;
	instructions[VM_XOR] = inst_xor;

	instructions[VM_NULL] = inst_null;
	instructions[VM_NNULL] = inst_nnull;
	instructions[VM_EQ ] = inst_eq;
	instructions[VM_NEQ] = inst_neq;
	instructions[VM_LT ] = inst_lt;
	instructions[VM_BT ] = inst_bt;
	instructions[VM_LEQ] = inst_leq;
	instructions[VM_BEQ] = inst_beq;

	instructions[VM_DROP] = inst_drop;
	instructions[VM_DUP] = inst_dup;
	instructions[VM_NIP] = inst_nip;
	instructions[VM_SWAP] = inst_swap;
	instructions[VM_OVER] = inst_over;
	instructions[VM_ROT] = inst_rot;
	instructions[VM_MROT] = inst_mrot;

	instructions[VM_PUSH] = inst_push;
	instructions[VM_POP] = inst_pop;

	instructions[VM_FETCH] = inst_fetch;
	instructions[VM_STORE] = inst_store;

	instructions[VM_JUMP] = inst_jump;
	instructions[VM_CJUMP] = inst_cjump;
	instructions[VM_CALL] = inst_call;
	instructions[VM_CCALL] = inst_ccall;
	instructions[VM_RET] = inst_ret;

	instructions[VM_EMIT] = inst_emit;
	instructions[VM_GET] = inst_get;
	instructions[VM_DEBUG] = inst_debug;

	instructions[VM_BYE] = inst_bye;
	// instructions[VM_] = inst_;
}

void vm_parse_args(int argc, char **argv) {
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-d")) {
			debug_flag = -1;
		} else if (!strcmp(argv[i], "-f")) {
			if (i+1 < argc) {
				filename_parameter = argv[++i];
			} else {
				printf("too few arguments for -f");
				exit(1);
			}
		} else {
			printf("unknown option: %s\n", argv[i]);
			exit(1);
		}
	}
}

int main(int argc, char **argv) {
	printf("scooma v0.0.11\n");
	vm_parse_args(argc, argv);
	vm_prepare();
	if (!vm_load(filename_parameter)) {
		printf("error on reading image file\n");
		return 1;
	}
	vm_run();
	return 0;
}
