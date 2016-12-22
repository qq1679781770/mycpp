#include <iostream>
#include<vector>
using namespace std;
bool isinclude(int a)
{
    while(a!=0)
    {
        if(a==1||a%10==1) return true;
        else a=a/10;
    }
    return false;
}

int count(vector<int>& number)
{
    int coun=0;
    for(int i=0;i<number.size();i++)
        if(isinclude(number.at(i)))  coun+=1;
    return coun;

}

int main()
{

    int n;
    cout<<"请输入一个正整数:";
    cin>>n;
    vector<int>num(n);
    for(int i=0;i<n;i++)
        num.push_back(i+1);
    cout<<count(num);
    return 0;
}

