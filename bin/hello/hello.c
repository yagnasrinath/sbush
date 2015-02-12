#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]) {
	int i=255;
		//char tmp1[12];
		//char *tmp = tmp1;
		char arg ;
		scanf("%c%x",&arg, &i);
		printf("%c,%x \n",arg,i);
		return 0;
}
