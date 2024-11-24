#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef __FRAMAC__
typedef signed long long ssize_t;
typedef unsigned long long uint64_t;
#else
#include <stdint.h>
#endif

char * secret;

struct s_state {
	ssize_t ip;
	uint64_t stack[8];
	ssize_t sp;
	uint64_t * memory;
	char * program;
	struct s_state * this;
	ssize_t psize;
} stt;

uint64_t pop(struct s_state * state) {
	if(state->sp > 0)
		state->sp--;
	return state->stack[state->sp];
}

void push(struct s_state * state, uint64_t v) {
	state->sp++;
	state->stack[state->sp-1] = v;
}

/*@ requires valid_program: valid_read_string(state->program);
*/
int step(struct s_state * state) {
	uint64_t a, b;
	char * program = state->program;
	if(state->ip >= state->psize || state->ip < 0)
		return 1;
	switch(program[state->ip]) {
		case ':': // noop
			break;
		case 'Q': // quit
			return 1;
		case '.':
			pop(state);
			break;
		case '+':
			a = pop(state);
			b = pop(state);
			push(state, a+b);
			break;
		case '-':
			b = pop(state);
			a = pop(state);
			push(state, a-b);
			break;
		case 'S':
			a = pop(state);
			b = pop(state);
			push(state, a);
			push(state, b);
			break;
		case '/':
			a = pop(state);
			b = pop(state);
			push(state, b/a);
			break;
		case '*':
			a = pop(state);
			b = pop(state);
			push(state, a*b);
			break;
		case 'P':
			a = pop(state);
			printf("%ld\n", a);
			break;
		case 'X':
			a = pop(state);
			printf("0x%08lx\n", a);
			break;
		case 'D':
			a = pop(state);
			push(state, a);
			push(state, a);
			break;
		case 'I':
			a = 0;
			for(; program[state->ip+1] >= '0' && program[state->ip+1] <= '9';state->ip++) {
				if(state->ip >= state->psize || state->ip < 0)
					return 1;
				a = a*10 + (program[state->ip+1] - '0');
			}
			push(state, a);
			break;
		case '?':
			fprintf(stderr, "ip=%zd sp=%zd stack=", state->ip, state->sp);
			for(a=0;a<8;a++)
				if(a == state->sp)
					fprintf(stderr, "[%08lx] ", state->stack[a]);
				else
					fprintf(stderr, "%08lx ", state->stack[a]);
			fprintf(stderr, "\n  mem ");
			for(a=0;a<32;a++)
				fprintf(stderr, "%ld/%lx ", a, state->memory[a]);
			fprintf(stderr, "\n");
			break;
		case '0':
			a = pop(state);
			state->memory[0] = a;
			break;
		case 'Z':
			push(state, state->memory[0]);
			break;
		case '>':
			b = pop(state); // addr
			a = pop(state); // val
			if(a < 0 || a > 31) {
				fprintf(stderr, "invalid access %lx\n", a);
				return 1;
			}
			state->memory[a] = b;
			break;
		case '<':
			a = pop(state); // addr
			if(a < 0 || a > 31) {
				fprintf(stderr, "invalid access %lx\n", a);
				return 1;
			}
			push(state, state->memory[a]);
			break;
		default:
			fprintf(stderr, "?? 0x%02x\n", program[state->ip]);
			return 1;
			break;
	}
	state->ip++;
	return 0;
}

/*@ requires valid_argv0: valid_read_string(argv[0]);
  @ requires valid_argv1: valid_read_string(argv[1]);
  @ requires valid_argv2: valid_read_string(argv[2]);
  @ requires argc >= 3;
 */
int main(int argc, char ** argv) {
	memset(&stt, 0, sizeof(stt));
	if(argc == 1) {
		char prg[1024];
		ssize_t rd = fread(prg, 1, 1023, stdin);
		if(rd < 0) {
			perror("read");
			return 1;
		}
		prg[rd] = 0;
		stt.program = prg;
		secret = "QSD3546SDRFG546";
	} else if(argc != 3) {
		printf("usage: %s 'program' 'secret'\n", argv[0]);
		return 1;
	} else {
		stt.program = argv[1];
		secret = argv[2];
	}
	stt.psize = strlen(stt.program);
	stt.this = &stt;
	stt.memory = malloc(32 * 8);
	if(stt.memory == NULL)
		return 2;
	fprintf(stderr, "&secret=%p &stt=%p &memory=%p\n", &secret, &stt, stt.memory);
	memset(stt.memory, 0, 32*8);
	while( step(&stt) == 0) {};
	return 0;
}

#ifdef __FRAMAC__
int frama_c_main() {
	char * args[3];
	char * buf = malloc(1024);
	if(buf == NULL)
		return 1;
	args[0] = "./interpreter";
	if(fgets(buf, 1023, stdin) == NULL)
		return 2;
	int ln = strlen(buf);
	//@ assert \valid(buf + (0..ln-1) );
	args[1] = buf;
	args[2] = "secret";
	return main(3, args);
}
#endif
