#include<iostream>
using namespace std;
struct Node
{ 
   int data;
   Node * next;
};
class list
{
private:
int a[10];
const int length;
Node *node;
public:
list();
~list(){}
void createNode(Node* &head);
Node * reverse(Node* head);
void visit(Node* head);
void print();
};
list::list():length(10)
{
  node=NULL;
  cout<<"请输入十个数字";
  for(int i=0;i<length;i++)
     cin>>a[i];
}
void list::createNode(Node* &head)
{
  Node *tear=head;
  for(int i=0;i<length;i++)
    {
       Node *add=new Node;
       add->data=a[i];
       add->next=NULL;
       if(tear==NULL)
         {
            tear=add;
            head=tear;
         }
       else
         {
           tear->next=add;
           tear=add;
         }
   }
}
Node* list::reverse(Node* head)
{
   if(head==NULL) return head;
   if(head->next==NULL) return head;
   Node *ph=reverse(head->next);
   head->next->next=head;
   head->next=NULL;
   return ph;
}
void list::visit(Node* head)
{
   if(head!=NULL)
    {
      cout<<head->data<<" ";
      visit(head->next);
    }
}
void list::print()
{
   createNode(node);
   visit(node);
   Node *p=reverse(node);
   cout<<endl;
   visit(p);
}
int main()
{
   list a;
   a.print();
   return 0;
}

