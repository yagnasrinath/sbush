#include <sys/kbsctochar.h>


int cnvrt_to_caps_if_necessary(char c, BOOL is_shift_on) {
	if(is_shift_on)
		return c- 32; // caps and small differ by 32;
	return c;
}


uint8_t handle_other_charcters(uint8_t c, BOOL is_shift_on) {
	if(!is_shift_on) {
		switch (c)  {
		case SC_BACKSLASH           :       return '\\';
		case SC_FORWARDSLASH        :       return '/';
		case SC_MINUS_UNDERSCORE    :       return '-';
		case SC_PLUS_EQUALS         :       return '=';
		case SC_OPEN_CURL           :       return '[';
		case SC_CLOSE_CURL          :       return ']';
		case SC_COLON               :       return ';';
		case SC_AT                  :       return '\'';
		case SC_LT                  :       return ',';
		case SC_GT                  :       return '.';
		case SC_TILDA               :       return '`';
		}
	}
	else {
		switch (c)  {
		case SC_BACKSLASH           :       return '|';
		case SC_FORWARDSLASH        :       return '?';
		case SC_MINUS_UNDERSCORE    :       return '_';
		case SC_PLUS_EQUALS         :       return '+';
		case SC_OPEN_CURL           :       return '{';
		case SC_CLOSE_CURL          :       return '}';
		case SC_COLON               :       return ':';
		case SC_AT                  :       return '\"';
		case SC_LT                  :       return '<';
		case SC_GT                  :       return '>';
		case SC_TILDA               :       return '~';
		}
	}
	return 0;
}

uint8_t get_asci_for_sc(uint8_t c, BOOL is_shift_on) {

	if(c == SC_SPACE) {
		return ' ';
	}
	if(c == SC_RETURN) {
		return '\n';
	}
	if(c == SC_TAB) {
		return '\t';
	}
	if(c == SC_BACKSPACE) {
		return '\b';
	}

	if(c >= SC_Q && c <= SC_P) {
		return cnvrt_to_caps_if_necessary("qwertyuiop"[c-SC_Q], is_shift_on);
	}

	if(c >= SC_A && c <= SC_L) {
		return cnvrt_to_caps_if_necessary("asdfghjkl"[c-SC_A], is_shift_on);
	}

	if(c >= SC_Z && c <= SC_M) {
		return cnvrt_to_caps_if_necessary("zxcvbnm"[c-SC_Z], is_shift_on);
	}

	if(c >= SC_ONE && c <= SC_NINE) {
		if (is_shift_on)
			return  "!@#$%^&*("[c-SC_ONE];
		return "123456789"[c-SC_ONE];
	}
	if( c == SC_ZERO) {
		if(is_shift_on)
			return ')';
		return '0';
	}
	return handle_other_charcters(c, is_shift_on) ;

}
