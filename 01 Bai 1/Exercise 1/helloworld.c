#include <stdio.h>
#define null 0
void printfln(char *ch)
{
  printf("%s",ch);
  printf("\n");
}
int main(void)
{
  int *i = null;
  printfln("Hello world!");
  return 0;
}
