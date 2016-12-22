#include<iostream>
#include<malloc.h>
using namespace std;
struct bitree
{
  int data;
  bitree * left;
  bitree * right;
};
class swaptree
{
private:
int *number;
int length;
int pos;
bitree * btree;
public:
void createtree(bitree* &root);
void swap(bitree* &a,bitree* &b);
void mitrro(bitree * root);
void visit(bitree *root);
void Btinsert(bitree* &root,int element);
swaptree(int n);
~swaptree(){delete [] number;}
void print();
};
swaptree::swaptree(int n)
{ 
  btree=NULL;
  length=n;
  pos=0;
  number=new int[length];
  cout<<"请输入"<<length<<"个数字:";
  for(int i=0;i<length;i++)
    cin>>number[i];
}
void swaptree::Btinsert(bitree* &root,int element)
{
   if(root==NULL)
     {
        root=new bitree;
        root->data=element;
        root->left=NULL;root->right=NULL;
     }
   else
     { 
       if(element<root->data)
          Btinsert(root->left,element);
       else Btinsert(root->right,element);
     }
}
void swaptree::createtree(bitree* &root)
{
  for(int i=0;i<length;i++)
      Btinsert(root,number[i]);
}
void swaptree::swap(bitree* &a,bitree* &b)
{
  bitree* t;
  t=a;
  a=b;
  b=t;
}
void swaptree::mitrro(bitree *root)
{
  if(root==NULL) return ;
  swap(root->left,root->right);
  mitrro(root->left);
  mitrro(root->right);
}
void swaptree::visit(bitree *root)
{
  if(root!=NULL)  
    {
       cout<<root->data<<" ";
       visit(root->left);
       visit(root->right);
    }
}
void swaptree::print()
{
  createtree(btree);
  visit(btree);
  mitrro(btree);
  visit(btree);
}
int main()
{ 
  swaptree a(8);
  a.print();
  return 0;
}
    
