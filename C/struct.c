#include <stdio.h>
struct complex
{
	int im, re;
};
struct complex  create_complex(int im, int re)
{
	struct complex temp;
	temp.im = im;
	temp.re = re;
	return temp;
}
int main()
{
	struct complex a,b;
	a.im = 1;
	a.re = 2;
	b = create_complex(a.im, a.re);
	printf(" %d %d\n", b.im, b.re);
//	if(a == b)
//		printf("a equal b\n");
	return 0;
}
