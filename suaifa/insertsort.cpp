#include <iostream>
#include<stdlib.h>
using namespace std;
class insertsort
{
private:
    int *number;
    int length;
public:
    insertsort(int a);
    ~insertsort(){delete []number;}
    void sort(int a[],int n);
    void print();
};
insertsort::insertsort(int a)
{
    length=a;
    number=new int[length];
    for(int i=0;i<length;i++)
    {
        number[i]=rand()%1024;
    }
}
void insertsort::sort(int a[], int n)
{
    for(int i=1;i<n;i++)
    {
        int key=a[i];
        int j=i-1;
        while(j>=0&&a[j]>key)
        {
            a[j+1]=a[j];
            j-=1;
        }
        a[j+1]=key;
    }
}
void insertsort::print()
{
    sort(number,length);
    for(int i=0;i<length;i++)
        cout<<number[i]<<endl;
}

int main()
{
    cout << "Hello World!" << endl;
    insertsort a(16);
    a.print();
    return 0;
}

