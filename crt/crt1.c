#include <stdlib.h>
#include<stdio.h>
int res;
int argc;
char**argv;
char **envp;

int main(int argc, char* argv[], char* envp[]);

void _start(void) {
	 uint64_t stack_breaker[0];
	argc = *(uint64_t*)(((uint64_t)stack_breaker)+sizeof(uint64_t));
	argv = (char**)(((uint64_t)stack_breaker)+2*sizeof(uint64_t));
	envp =(char**)(((uint64_t)stack_breaker)+3*sizeof(uint64_t)+sizeof(uint64_t)*argc);
	int res = main(argc, argv, envp);
	exit(res);
}
