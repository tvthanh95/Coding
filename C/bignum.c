#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 102
void make_num_from_str(const char *st, char num[SIZE])
{
	int i = 0;
	while(st[i] != '\0')
	{
		i += 1;
	}
	int size = i;
	int j = SIZE - 2;
	num[SIZE - 1] = '\0';
	for(i = size; i >= 0; i--)
	{
		num[j] = st[i];
		j -= 1; 
	}
	while(j >= 0)
	{
		num[j] = '0';
		j -= 1;
	}
}
void make_zero(char num[SIZE])
{
	int i = 0;
	num[SIZE - 1] = '\0';
	for(i = 0; i < SIZE -1 ; i++)
	{
		num[i] = '0';
	}
}
void print_big_num(char num[SIZE])
{
	int i = 0;
	while(num[i] == '0')
	{
		i +=1;
	}
	while(i <= SIZE - 2)
	{
		printf("%c", num[i]);
		i += 1;
	}
	printf("\n");
}
void add_big_num(char num1[SIZE], char num2[SIZE], char sum[SIZE])
{
	int i, carry;
	carry = 0;
	for(i = SIZE - 2; i >= 0; i--)
	{
		carry = (num1[i] - '0') + (num2[i] - '0') + carry;
		if(carry >= 10)
		{
			sum[i] = carry - 10 + '0';
			carry = 1;

		}
		else
		{
			sum[i] = carry + '0';
			carry = 0;
		}
	}
}
void mul_simple(char num[SIZE], int times, char res[SIZE])
{
	int i, carry;
	carry = 0;
	for(i = SIZE  - 2; i >= 0; i--)
	{
		carry = (num[i] - '0') * times;
		if(carry >= 10)
		{
			res[i] = (carry % 10) + '0';
			carry = carry / 10 ;
		}
		else
		{
			res[i] = carry + '0';
			carry = 0;
		}
	}
}
void shift_left(char num[SIZE])
{
	int i = 0;
	while(num[i] == '0')
	{
		i += 1;
	}
	i = i - 1;
	while(i < SIZE -2)
	{
		num[i] = num[i+1];
		i += 1;
	}
	num[i] = '0';
}
void mul_big_num(char num1[SIZE], char num2[SIZE], char res[SIZE])
{
	int i = 0, times;
	char temp[SIZE];
	make_zero(temp);
	while(num2[i] == '0')
	{
		i += 1;
	}
	times = num2[i] - '0';
	mul_simple(num1,times,temp);
	while(i <= SIZE - 1)
	{
		add_big_num(temp, res, res);
		i += 1;
		times = num2[i] - '0';
		mul_simple(num1,times,temp);
		shift_left(temp);
	}
	add_big_num(temp,res,res);
}
int main()
{
	char a[SIZE], b[SIZE], c[SIZE];
	make_num_from_str("12",a);
	make_num_from_str("12",b);
	make_zero(c);
	//mul_simple(a,2,c);
	mul_simple(a,7,b);
	print_big_num(b);
	return 0;
}
