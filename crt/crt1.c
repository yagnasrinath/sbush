#include <stdlib.h>

void _start(void) {
	int argc = 1;
	char* argv[] = {"hello"};
	char** envp = NULL;
	int res;
	res = main(argc, argv, envp);
	exit(res);
}
