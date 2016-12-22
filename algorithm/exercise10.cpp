#include<iostream>
using namespace std;
int continumax(char *outputstr,char *inputstr)
{
    int len=0;
    int max=0;
    char *pstart=NULL;
    while((*inputstr)!='\0')
    {
        if((*inputstr)<='9'&&(*inputstr)>='0')
            len++;
            else
            {
                if(len>max)
                {
                    max=len;
                    pstart=inputstr-len;
                }
                len=0;
            }
            inputstr++;
    }
    for(int j=0;j<max;j++)
        *outputstr++=*pstart++;
    *outputstr='\0';
    return max;
}
int main()
{
    char str[]="abcsas122287127hsdbdbs32323bbdsb323232";
    char *outputstr=new char[1024];
    cout<<continumax(outputstr,str);
    return 0;
}



       
