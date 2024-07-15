#include <stdio.h>
#include "sub.h"
#include "sum.h"
int main(int argc, char *argv[])
{
    int a = 1;
    int b = 2;
    printf("sum %d + %d = %d\n",a,b,sum(a,b));
    printf("sub %d - %d = %d\n",a,b,sub(a,b));
    return 0;
}