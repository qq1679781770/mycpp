#include<iostream>
#include<stdlib.h>
using namespace std;
class qqsort
{
private:
    int *a;
    int length;
public:
    qqsort(int b);
    ~qqsort(){delete []a;}
   void quicksort(int b[],int p,int r);
   int preparation(int c[],int p,int r);
   void print();
   void exchange(int &a,int &b);
};
qqsort::qqsort(int b)
{
    length=b;
    a=new int [length];
    for(int i=0;i<length;i++){
        a[i]=rand()%1024;
    }
}

void qqsort::exchange(int &a, int &b)
{
    int t=a;
    a=b;
    b=t;
}
int qqsort::preparation(int c[], int p, int r)
{
    int x=c[r];
    int i=p-1;
    for(int j=p;j<r;j++)
    {
        if(a[j]<x)
        {
            i+=1;
            exchange(a[i],a[j]);
        }
    }
    exchange(a[i+1],a[r]);
    return i+1;
}
void qqsort::quicksort(int b[], int p, int r)
{
    if(p<r){
        int q=preparation(a,p,r);
        quicksort(b,p,q-1);
        quicksort(b,q+1,r);
    }
}
void qqsort::print()
{
    quicksort(a,0,length-1);
    for(int i=0;i<length;i++)
    {
        cout<<a[i]<<endl;
    }
}
int main()
{
   cout<<"hello world"<<endl;
  qqsort a(15);
  a.print();

}
