#include<string.h>
#include<errno.h>
char *strcpy ( char *__restrict dest, const char *__restrict src) {
    char* orgdest = dest;
    while((*dest = *src)){
        dest++;
        src++;
    }
    return orgdest;
}


/* Copy no more than N characters of SRC to DEST.  */
char *strncpy (char *__restrict dest, const char *__restrict src, const size_t n) {
    char* orgdest =dest;
    int i=0;
    while((i++ < n)&&(*dest++ = *src++));

    if(i<n) {
        dest[i] = '\0';
    }
    return orgdest;           
}


void *memset(void* input,int c, size_t size)
{
    int i=0;
    unsigned char* p = input;
    for(i=0;i<size;i++)
    {
        *p = (unsigned char)c;
        p++;
    }
    return input;
}


int strcmp (const char *s1, const char *s2) {
	while(*s1&&*s2){
        if(*s1-*s2){
            return *s1-*s2;
        }
        s1++;
        s2++;
    }
    if((*s1)&&!(*s2)){
        return *s1;
    }
    else if((*s2)&&!(*s1)) {
        return -(*s2);
    }

    return 0;
}


int strncmp (const char *s1, const char *s2, size_t n){
    int i=0;
    while((*s1&*s2)&&(i++<n)){
        if(*s1-*s2){
            return *s1-*s2;
        }
        s1++;
        s2++;
    }
    if((*s1)&&!(*s2)){
        return *s1;
    }
    else if((*s2)&!(*s1)) {
        return -*s2;
    }
    return 0;

}

char *strcat (char *__restrict dest, const char *__restrict src) {
    char *orgdest = dest;
    while(*dest){
        dest++;
    }
    while(*src){
        *dest++ =*src++;
    }
    *dest='\0';
    return orgdest;
}

char *strncat (char *__restrict dest, const char *__restrict src, size_t n) {
    char *orgdest = dest;
    while(*dest){
        dest++;
    }
    int i=0;
    while(*src &&(i++<n)){
        *dest++ =*src++;
    }
    if(*src)
        *dest='\0';
    return orgdest;
}

size_t strlen (const char *s) {
    size_t i=0;
    while(*s++) i++;
    return i;
}

/*
const char *strstr(const char *haystack, const char *needle)
{
    const char *needle_temp = needle;
    if(!*needle_temp)
    {
        return haystack;
    }
    int size_needle = 0;
    while(*(needle_temp++)) size_needle++;
    int index = 0;
    needle_temp = needle;
    const char *haystack_temp = haystack;
    while(*haystack_temp)
    {
        while(*needle_temp)
        {
            if(*needle_temp != haystack_temp[index])
            {
                break;
            }
            index++;
            needle_temp++;
        }
        if(index == size_needle)
        {
            return haystack_temp;
        }
        index = 0;
        needle_temp = needle;
        haystack_temp++;
    }
    return 0;
}
*/
char *strstr(char *haystack, char *needle)
{
    char *needle_temp = needle;
    if(!*needle_temp)
    {
        return haystack;
    }
    int size_needle = 0;
    while(*(needle_temp++)) size_needle++;
    int index = 0;
    needle_temp = needle;
    char *haystack_temp = haystack;
    while(*haystack_temp)
    {
        while(*needle_temp)
        {
            if(*needle_temp != haystack_temp[index])
            {
                break;
            }
            index++;
            needle_temp++;
        }
        if(index == size_needle)
        {
            return haystack_temp;
        }
        index = 0;
        needle_temp = needle;
        haystack_temp++;
    }
    return 0;
}

int32_t atoi(char *p)
{
	int sign =1;
	if (p[0] == '-' || p[0] == '+') {
		if(p[0] == '-' )
			sign = -1;
		p++;
	}
	int i=0, output = 0;
	while (p[i]!= '\0') {
		if (p[i] >= 48 && p[i] <= 57) {
			output = 10*output+p[i]-'0';
			i++;
		} else {
			return 0;
		}

	}
	return output*sign;
}

extern const char *sys_errlist[]; 
const char *strerror(int errnum)
{
    return  sys_errlist[errno];
}
