#include <stdio.h>
struct Complex
{
	int im;
	int re;
};
struct Complex make_Complex(int im, int re)
{
	struct Complex temp;
	temp.im = im;
	temp.re = re;
	return temp;
}
struct Complex add(struct Complex x, struct Complex y)
{
	struct Complex temp;
	temp.im = x.im + y.im;
	temp.re = x.re + y.re;
	return temp;
}
void print_Complex(struct Complex x)
{
	printf("%d + %di\n",x.im, x.re);
}
int main()
{
	struct Complex (*ptr1)(int, int);
	ptr1 = make_Complex;
	struct Complex c = ptr1(2,5);
	struct Complex d = ptr1(3,6);
	struct Complex (*ptr2)(struct Complex, struct Complex);
	ptr2 = add;
	//print_Complex(ptr2(c, d));
	struct Complex (*ptr[2])(int, int);
	ptr[0] = make_Complex;
	ptr[1] = make_Complex;
	print_Complex(ptr2(ptr[0](2,0), ptr[1](-2,0)));
	return 0;
}
