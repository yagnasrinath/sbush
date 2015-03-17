/*
 * kbtosc.h
 *
 *  Created on: Mar 1, 2015
 *      Author: ravichandrasadineni
 */

#ifndef SBUSH_INCLUDE_SYS_KBSCTOCHAR_H_
#define SBUSH_INCLUDE_SYS_KBSCTOCHAR_H_
#include<sys/defs.h>
enum kbtosc {
	DUMMY, //0
	SC_ESC, //1
	SC_ONE, //2
	SC_TWO, //3
	SC_THREE, //4
	SC_FOUR, //5
	SC_FIVE, //6
	SC_SIX, //7
	SC_SEVEN, //8
	SC_EIGHT, //9
	SC_NINE,// 10
	SC_ZERO, //11
	SC_MINUS_UNDERSCORE, //12
	SC_PLUS_EQUALS, //13
	SC_BACKSPACE, //14
	SC_TAB, //15
	SC_Q, //16
	SC_W, //17
	SC_E, //18
	SC_R, //19
	SC_T, //20
	SC_Y, //21
	SC_U, //22
	SC_I, //23
	SC_O, //24
	SC_P, //25
	SC_OPEN_CURL, //26
	SC_CLOSE_CURL, //27
	SC_RETURN,// 28
	SC_CTRL, //29
	SC_A, //30
	SC_S, //31
	SC_D, //32
	SC_F, //33
	SC_G, //34
	SC_H, //35
	SC_J, //36
	SC_K, //37
	SC_L, //38
	SC_COLON, //39
	SC_AT, //40
	SC_TILDA, //41
	SC_LEFTSHIFT, //42
	SC_BACKSLASH, //43
	SC_Z, //44
	SC_X, //45
	SC_C , //46
	SC_V, //47
	SC_B, //48
	SC_N, //49
	SC_M , //50
	SC_LT, //51
	SC_GT, //52
	SC_FORWARDSLASH, //53
	SC_RIGHTSHIFT, //54
	SC_PRINTSCREEN, //55
	SC_ALT, //56
	SC_SPACE, //57
	SC_CAPSLOCK, //58
	SC_F1, //59
	SC_F2, //60
	SC_F3, //61
	SC_F4, //62
	SC_F5, //63
	SC_F6, //64
	SC_F7, //65
	SC_F8, //66
	SC_F9, //67
	SC_F10 , //68
	SC_NUMLOCK, //69
	SC_SCROLLLOCK, //70
	SC_HOME, //71
	SC_UP, //72
	SC_PGUP,//73
	SC_NUMPAD_MINUS, //74
	SC_NUMPAD_4, //75
	SC_NUMPAD_5, //76
	SC_NUMPAD_6, //77
	SC_NUMPAD_PLUS, //78
	SC_NUMPAD_1, //79
	SC_NUMPAD_2, //80
	SC_NUMPAD_3, //81
	SC_NUMPAD_0, //82
	SC_NUMPAD_PERIOD, //83
	SC_F11  = 87,//87
	SC_F12= 88, //88
	SC_MAX = 96, //96
};
uint8_t get_asci_for_sc(uint8_t b, BOOL shift);
#endif /* SBUSH_INCLUDE_SYS_KBSCTOCHAR_H_ */
