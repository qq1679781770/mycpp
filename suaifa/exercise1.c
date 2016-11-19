#include<stdio.h>
int main()
{
int i=0;
int m,x,n;
int j=1;
int a[10];
char p[100];
//char c;
while((p[i]=getchar())!='*')
{
  i=i+1;
  
}
p[i]='\0';
a[0]=-1;
for(n=0;n<i;n++)
{
if(p[n]==' ') 
{
 a[j]=n;
 j++;
}
}
a[j]=i;
for(m=j;m>0;m--)
{
for(x=a[m-1]+1;x<a[m];x++)
  printf("%c",p[x]);
printf(" ");
}
return 0;
}
