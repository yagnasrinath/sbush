/*
 * sys_generic_util.c
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */

#include <sys/sys_generic_util.h>
#include <sys/sbunix.h>


uint64_t oct_to_dec(uint64_t n) {
	uint64_t dec = 0, i = 0, curpos;
	for(;n != 0;n/=10,++i) {
		curpos = n % 10;
		dec += curpos * pow(8, i);
	}
	return dec;
}

uint64_t pow(uint64_t x, uint64_t y)
{
	uint64_t temp;
    if( y == 0)
       return 1;
    temp = pow(x, y/2);
    if (y%2 == 0)
        return temp*temp;
    else
    {
        if(y > 0)
            return x*temp*temp;
        else
            return (temp*temp)/x;
    }
}

