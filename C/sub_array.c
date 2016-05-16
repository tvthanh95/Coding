#include <stdio.h>
/*
sum(i,j) = sum(0,j) - sum(0,i-1)
=> tim i sao cho sum(0, i-1) nho nhat
khoi tao:
sum = a[0]
sum_min = a[0]
low = 0
high = 0
sum_max = a[0]
for(j = 1; j < size; j++)
{
    sum+=a[j];
    if(sum - sum_min > sum_max)
        {
            sum_max = sum -sum_min;
            high = j;
        }
    if(sum < sum_min)
    {
        sum_min = sum;
        low = j;
    }
}

*/
struct result
{
    int low,high;
};
int check_negative(int array[], int size)
{
    int negative = 1;
    int i;
    for(i = 0; i < size; i++)
    {
        if(array[i] > 0)
        {
            negative = 0;
            break;
        }
    }
    return negative;
}
int find_max_index(int array[], int size)
{
    int i, max = 0;
    for(i = 1; i < size; i++)
    {
        if(array[max] < array[i])
            max = i;
    }
    return max;
}
struct result find_max_sub(int array[], int size)
{
    struct result temp;
    if(check_negative(array, size) == 1)
    {
        temp.low = temp.high = find_max_index(array, size);
        return temp;
    }
    temp.low = 0;
    temp.high = 0;
    int i, sum, sum_max, sum_min;
    sum = sum_max = array[0];
    if(array[0] > 0)
        sum_min = 0;
    else
        sum_min = array[0];
    
    for(i = 1; i < size; i++)
    {
        sum += array[i];
        if(sum - sum_min > sum_max)
        {
             sum_max = sum - sum_min;
             temp.high = i;
        }
        if(sum < sum_min)
        {
            sum_min = sum;
            temp.low = i;
        }
    }
    return temp;
}
int main()
{
    int array[] = {4, -1, 3,-5};
    int size = sizeof(array)/sizeof(int);
    struct result res;
    res = find_max_sub(array, size);
    printf("%d %d\n", res.low, res.high);
    return 0;
}