#include <stdio.h>
struct complex
{
	int im;
	int re;
};
struct complex make_complex(int im, int re)
{
	struct complex temp;
	temp.im = im;
	temp.re = re;
	return temp;
}
struct complex add(struct complex x, struct complex y)
{
	struct complex temp;
	temp.im = x.im + y.im;
	temp.re = x.re + y.re;
	return temp;
}
void print_complex(struct complex x)
{
	printf("%d + %di\n",x.im, x.re);
}
int main()
{
	struct complex (*ptr1)(int, int);
	ptr1 = make_complex;
	struct complex c = ptr1(2,5);
	struct complex d = ptr1(3,6);
	struct complex (*ptr2)(struct complex, struct complex);
	ptr2 = add;
	//print_complex(ptr2(c, d));
	struct complex (*ptr[2])(int, int);
	ptr[0] = make_complex;
	ptr[1] = make_complex;
	print_complex(ptr2(ptr[0](2,0), ptr[1](-2,0)));
	return 0;
}
