#include<iostream>
using namespace std;
struct list
{
  int data;
  list *next;
};
class nodelist
{
private:
  list *  head;
  int * number;
  int length;
  int key;
public:
  nodelist(int n);
  void  search(list * p);
  void visit(list * p);
  void  createlist(list* &p);
  void print();
  ~nodelist();
};
nodelist::nodelist(int n)
{
  length=n;
  number=new int[length];
  cout<<"请输入"<<length<<"个数字:"<<endl;
  for(int i=0;i<length;i++)
      cin>>number[i];
}
void nodelist::createlist(list* &p)
{
   p=NULL;
   list *rear=NULL;
   for(int i=0;i<length;i++)
     { 
       list *add=new list;
       add->data=number[i];
       add->next=NULL;
       if(rear==NULL)
        { rear=add;
         p=rear;}
       else
         {
            rear->next=add;
            rear=add;
         }
     }
}
         
   
void nodelist::search(list *p)
{
  cout<<"请输入倒数第几个节点:(最后一个为0)";
  cin>>key;
  int num=key;
  list *tear=p;
  for(;key>0;key--)
     if(tear!=NULL) tear=tear->next;
  while(tear->next!=NULL)
    {
       head=head->next;
       tear=tear->next;
    }
  cout<<"第"<<num<<"个节点值为"<<head->data; 
}
void nodelist::visit(list *p)
{ 
   while(NULL!=p)
  {
    cout<<p->data;
    p=p->next;
  }
}
void nodelist::print()
{
  createlist(head);
 // visit(head);
  search(head);
 
}
nodelist::~nodelist()
{
delete [] number;
}
int main()
{
  nodelist a(10);
  a.print();
  return 0;
}

