#include <iostream>
#include<stdlib.h>
#include<stack>
using namespace std;
class heap
{
private:
    int *number;
    int length;
    stack<int>data;
public:
    heap(int b);
    ~heap(){delete []number;}
    void exchange(int& a,int &b0);
    void maxheap(int b[],int i,int n);
    void buildheap(int c[],int n);
    void haeapsort(int  d[],int n);
    void print();
};
heap::heap(int b)
{
    length=b;
    number=new int[length+1];
    number[0]=length;
    for(int i=1;i<=length;i++)
    {
        number[i]=rand()%1024;
    }
}
void heap::exchange(int &a, int &b)
{
    int t=a;
    a=b;
    b=t;
}
void heap::maxheap(int b[], int i, int n)
{

    int largest=i;
    int left=2*i;
    int right=2*i+1;
    if(left<=n)
       { if(b[left]>b[largest]) largest=left;}
    if(right<=n)
      {  if(b[right]>b[largest]) largest=right;}
    if(largest!=i)
    {
        exchange(b[i],b[largest]);
        maxheap(b,largest,n);
    }
}
void heap::buildheap(int c[], int n)
{
    for(int j=n/2;j>=1;j--)
        maxheap(c,j,n);
}
void heap::haeapsort(int d[], int n)
{
    buildheap(d,n);
    for(int i=n;i>=1;i--)
    {
        exchange(d[1],d[i]);
        data.push(d[i]);
                maxheap(d,1,i-1);
    }
}
void heap::print()
{

    for(int i=1;i<=length;i++)
        cout<<number[i]<<endl;
     haeapsort(number,length);
    cout<<"*****"<<data.size()<<"******"<<endl;
    while(!data.empty())
    {
        cout<<data.top()<<endl;
        data.pop();
    }
}

int main()
{
    cout << "Hello World!" << endl;
    heap a(16);
    a.print();
    return 0;
}

