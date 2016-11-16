#include <iostream>
#include<stdlib.h>
using namespace std;
class merge
{
private:
    int *number;
    int length;
public:
    merge(int a);
    ~merge(){delete []number;}
    void mergersort(int a[],int p,int r);
    void merger(int a[],int p,int q,int r);
    void print();
};
merge::merge(int a)
{
    length=a;
    number=new int[length];
    for(int i=0;i<length;i++)
        number[i]=rand()%1024;
}
void merge::merger(int a[], int p, int q, int r)
{
    int cur1=0;
    int cur2=0;
    int n1=q-p+1;
    int n2=r-q;
    int m1[n1];
    int m2[n2];
    for(int i=0;i<n1;i++)
        m1[i]=a[p+i];
    for(int j=0;j<n2;j++)
        m2[j]=a[q+j+1];
    m1[n1]=m2[n2]=1024;

    for(int k=p;k<=r;k++)
    {
        if(m1[cur1]<m2[cur2])
           {
             a[k]=m1[cur1];
            cur1+=1;
        }
        else
        {
            a[k]=m2[cur2];
            cur2+=1;
        }
    }

}
void merge::mergersort(int a[], int p, int r)
{
    if(p<r)
    {
        int q=(p+r)/2;
        mergersort(a,p,q);
        mergersort(a,q+1,r);
        merger(a,p,q,r);
    }
}
void merge::print()
{
    mergersort(number,0,length-1);
    for(int i=0;i<length;i++)
        cout<<number[i]<<endl;
}

int main()
{
    cout << "Hello World!" << endl;
    merge a(16);
    a.print();
    return 0;
}

