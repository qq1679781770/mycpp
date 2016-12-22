#include<stdio.h>
#include<memory.h>
void firstsingle(char *str)
{
    int a[255];
    memset(a, 0, 255*sizeof(int));
    char *p=str;
    while((*p)!='\0')
    {
        a[*p]+=1;
        p++;
    }
    for(int i=0;i<255;i++)
        if(a[i]==1)
            printf("%c",i);
}
int main()
{
    char s[]={'a','b','b','\0'};
    firstsingle(s);
    return 0;
}
