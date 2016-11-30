#include <iostream>
#include<vector>
#include<cmath>
#include<stdlib.h>
#include<algorithm>
using namespace std;
void exchange(int &a,int &b)
{
    int t=a;
    a=b;
    b=t;
}
bool Sort(int a,int b)
{
    return a<b;
}
int  subtract(vector<int>& a,vector<int>& b)
{
    int key=a.at(0)-b.at(0);
    for(int i=1;i<a.size();i++)
    {
        int divid=a.at(i)-b.at(i);
        if(abs(divid+key)<abs(key))
        {
            key=divid+key;
        }
        else
        {
            exchange(a.at(i),b.at(i));
            key=key-divid;
        }
    }
    return key;
}

int main()
{
    vector<int>::iterator iter;
    int n;
    cout<<"请输入一个数字";
    cin>>n;
    vector<int>a;
    vector<int>b;
    for(int i=0;i<n;i++)
       {
        int data=rand()%1024;
        a.push_back(data);
       }
    for(int j=0;j<n;j++)
    {
        int data=rand()%1024;
        b.push_back(data);
    }
   sort(a.begin(),a.end(),Sort);
    sort(b.begin(),b.end(),Sort);
    cout<<"交换前a"<<endl;
 for(iter=a.begin();iter!=a.end();iter++)
     cout<<*iter<<" ";
    cout<<"交换前b"<<endl;
  for(iter=b.begin();iter!=b.end();iter++)
      cout<<*iter<<" ";
  cout<<endl;
 cout<<"交换后差为:" <<subtract(a,b);
  cout<<"交换后a"<<endl;
for(iter=a.begin();iter!=a.end();iter++)
   cout<<*iter<<" ";
  cout<<"交换后b"<<endl;
for(iter=b.begin();iter!=b.end();iter++)
    cout<<*iter<<" ";

    return 0;
}

