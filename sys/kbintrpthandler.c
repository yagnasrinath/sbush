#include <sys/kbsctochar.h>
#include <sys/scrn.h>
#include <sys/system.h>
#include<sys/sbunix.h>
#include<sys/idt.h>
#include<sys/utils/kstring.h>
static  volatile uint16_t special_keys =0;
static  volatile  uint16_t LEFT_SHIFT_PRESS = 1;
static  volatile  uint16_t LEFT_SHIFT_RELEASE = ~(1);
static  volatile  uint16_t RIGHT_SHIFT_PRESS = 2;
static  volatile  uint16_t RIGHT_SHIFT_RELEASE = ~(2);
static  volatile  uint16_t CTRL_PRESS = 4;
static  volatile  uint16_t CTRL_RELEASE = ~(4);
static volatile uint16_t flag =0;
static volatile uint64_t last_addr;
static volatile uint64_t counter;
static volatile uint64_t num_char_to_read;
static volatile char buf[1024];



void init_keyboard()
{
	special_keys = 0;
	last_addr = 0;
	flag = counter = 0;
}

int gets(uint64_t addr, uint64_t length)
{
	char *user_buf=(char*)addr;
	//int numCharRead = 0;
	num_char_to_read = length;
	flag=1;
	__asm__ __volatile__ ("sti");
	last_addr=get_current_addr();
	while (flag == 1);
	__asm__ __volatile__ ("cli");
	kmemcpy((void *)user_buf, (void *)buf, counter);
	int count = counter;
	counter = 0;
	num_char_to_read = 0;
	return count;
}

void print_char_pressed(char c) {
	/*char output[3];
	if(c == '\t') {
		output[0] = '\\';
		output[1] ='t';
		output[2] ='\0';
	}
	else if(c == '\n') {
		output[0] = '\\';
		output[1] ='n';
		output[2] ='\0';
	}
	else if(c == '\b') {
		output[0] = '\\';
		output[1] ='b';
		output[2] ='\0';
	}
	else {
		output[0]=c;
		output[1]=' ';
		output[2]='\0';
	}
	 */
}

BOOL valid_ascii(int val)
{
	if(val==0||val==1||(val>=54&&val<=56)||(val>=58&&val<=73))
	{
		return FALSE;
	}
	return TRUE;
}


void  kb_intrpt_handler( struct isr_nrm_regs  stack) {
	uint8_t b = inportb(0x60);

	if(b == SC_LEFTSHIFT) {
		special_keys = special_keys | LEFT_SHIFT_PRESS;
	}
	else  if(b == (SC_LEFTSHIFT | 0X80)) {
		special_keys = special_keys & LEFT_SHIFT_RELEASE;
	}
	else if (b == SC_RIGHTSHIFT ) {
		special_keys = special_keys | RIGHT_SHIFT_PRESS;
	}
	else if (b == (SC_RIGHTSHIFT  |0X80)) {
		special_keys = special_keys & RIGHT_SHIFT_RELEASE;
	}
	else if(b == SC_CTRL ){
		special_keys = special_keys	| CTRL_PRESS;
	}
	else if(b == (SC_CTRL | 0x80)){
		special_keys = special_keys & CTRL_RELEASE;
	}
	else if( b < 0X80){
		BOOL is_shift_pressed = (special_keys & LEFT_SHIFT_PRESS) ||
				(special_keys & RIGHT_SHIFT_PRESS);
		BOOL is_ctrl_pressed = (special_keys & CTRL_PRESS);
		if(flag == 1 && is_ctrl_pressed && (b == SC_C || b == SC_D)){
				putch('\n');
				counter = 0;
				flag= 0;
		}
		else{
			uint64_t val = get_asci_for_sc(b,is_shift_pressed);
			//kprintf("in else %c\n",b);
			if (flag == 1) {
				if (val == '\n') {
					buf[counter++] = '\n';
					buf[counter++] = '\0';
					putch(val);
					flag = 0;
				}
				else if (val == '\b') {
					if (get_current_addr() > last_addr) {
						putch(val);
						counter--;
					}
				} else {
					if(valid_ascii(val))
					{
						buf[counter] = val;
						counter += 1;
					}
					putch(val);
					if(counter == num_char_to_read) {
						flag = 0;
					}
				}
			}
		}
	}
}
