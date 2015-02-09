#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int printf(const char *format, ...) {
	va_list val;

	va_start(val, format);
	int num = 0;

	num = write(1, format, 11);



	return num;
}
