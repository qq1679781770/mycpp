#include<iostream>
using namespace std;
void Bagproblem(int n,int m,int *flag,int len)
{
    if(n<1||m<1) return;
    if(n<m)
    {
        flag[n-1]=1;
        Bagproblem(n-1,m-n,flag,len);
        flag[n-1]=0;
        Bagproblem(n-1,m,flag,len);
    }
    else
    {
        flag[m-1]=1;
        for(int i=0;i<len;i++)
            if(flag[i]!=0) cout<<i+1<<" ";
            cout<<endl;
        flag[m-1]=0;
        Bagproblem(m-1,m,flag,len);
    }
}
int main()
{
    int n,m;
    cout<<"请输入n";
    cin>>n;
    cout<<"请输入m";
    cin>>m;
    int flag[n]={0};
    int len=n;
    Bagproblem(n,m,flag,len);
    return 0;
}

