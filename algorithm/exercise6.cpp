#include<iostream>
#include<queue>
using namespace std;
struct Bitree
{
   int data;
   Bitree *left;
   Bitree *right;
};
class btree
{
private:
int *number;
int length;
int pos;
Bitree *p;
public:
btree();
~btree(){delete []number;}
void createtree(Bitree* &root);
void print();
};
btree::btree()
{
  length=15;
  pos=0;
  number=new int[length];
  cout<<"请输中序排序15个数字";
  for(int i=0;i<length;i++)
    cin>>number[i];
}
void btree::createtree(Bitree* &root)
{
  if(number[pos]==0)
   {
      root=NULL;
      pos+=1;
   }
  else
   {
     root=new Bitree;
     root->data=number[pos];
     pos+=1;
     createtree(root->left);
     createtree(root->right);
  } 
}
void btree::print()
{
  createtree(p);
  queue<Bitree *>myqueue;
  myqueue.push(p);
  while(!myqueue.empty())
  {
    Bitree *node=myqueue.front();
    myqueue.pop();
    cout<<node->data;
    if(node->left!=NULL) myqueue.push(node->left);
    if(node->right!=NULL) myqueue.push(node->right);
  }
}
int main()
{
  btree a;
  a.print();
  return 0;
}    
