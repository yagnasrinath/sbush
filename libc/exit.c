#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void exit(int status) {
	 asm("movl $1,%%rax;"         /* SYS_exit is 1 */
	     "xorl %%rbx,%%rbx;"      /* Argument is in ebx, it is 0 */
	     "int  $0x80"            /* Enter kernel mode */
	  );

}

