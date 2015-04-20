#include<stdio.h>

struct bp
{
	char a:4;
	char b:4;
};

int main()
{
	struct bp bp;
	bp.a=1;
	bp.b=2;
	int *a = (int*)&bp;		
	printf("%d\n",*a);
}
