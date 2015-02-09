#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void exit(int status) {
	 asm("movw $1,%%rax;"
			 "xorw %%rbx,%%rbx;"
			 "int  $0x80;"
			 :
			 :
			 :
	  );

}

