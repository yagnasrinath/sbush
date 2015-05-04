#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[], char* envp[]) {
	char buf[255];
	memset(buf,'\0',255);
	getcwd(buf,255);
	printf("%s\n",buf);
	return 0;
}
