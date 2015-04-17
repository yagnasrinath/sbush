/*
 * sys_generic_util.c
 *
 *  Created on: Mar 31, 2015
 *      Author: ravichandrasadineni
 */

#include <sys/sys_generic_util.h>



int32_t oct_to_dec(int n) {
	int dec = 0, i = 0, curpos;
	for(;n != 0;n/=10,++i) {
		curpos = n % 10;
		dec += curpos * pow(8, i);
	}
	return dec;
}

int32_t pow(int base, int power)
{
	if(power == 0){
		return 1;
	}
	power = power/2;
	int result =0;
	if(power%2 == 1) {
		result = pow(base,power);
		result = result*result* base;
	}
	else {
		result = pow(base,power);
		result =result*result;
	}


	return result;
}
