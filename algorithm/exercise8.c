#include<stdio.h>
int main()
{
    int a[9]={1,2,3,4,5,6,7,8,9};
    int length=9;
    int k;
    int r;
    int count=0;
    int i=0;int j=1;
    printf("每次去除第几个数字:");
    scanf("%d",&k);
    while(count!=8)
     {  
        i++;
        if(i>8) i-=9; 
        if(a[i]!=0)
            {
                j++;
                if(j%k==0)
                  {
                      a[i]=0;
                      count+=1;
                  }
            }
     }
     for(r=0;r<length;r++)
         if(a[r]!=0) 
             printf("%d",a[r]);
         return 0;
}


