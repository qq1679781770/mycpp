#include<iostream>
#include<malloc.h>
using namespace std;
typedef struct nodetree
{
  char  data;
  nodetree *left;
  nodetree *right;
}nodetree,*bitree ;
class  btree
{
private:
bitree p;
int pos;
char  *number;
int length;
public:
btree(int n);
~btree(){delete []number;};
void createtree(bitree &node);
int max(int a,int b);
int maxdistance(bitree root);
int helper(bitree root,int &depth);
void visitree(bitree root);
void print();
};
btree::btree(int n)
{
  length=n;
  pos=0;
  number=new char[15];
  cout<<"请输入数据:";
  number[0]='a';number[1]='b';number[2]='d';number[3]='#';number[4]='g';number[5]='#';number[6]='#';number[7]='e';
  number[8]='#';number[9]='#';number[10]='c';number[11]='f';number[12]='#';number[13]='#';number[14]='#';
 //for(int i=0;i<length;i++)
  //{
    //cin>>number[i];
 // }
}
int  btree::max(int a,int b)
{
  return (a>b?a:b);
}
void btree::createtree(bitree &node)
{
 if(number[pos]=='#')
  {
   node=NULL;
   pos+=1;
}
 else
  {
     node=new nodetree;
     node->data=number[pos];
     pos+=1;
     createtree(node->left);
     createtree(node->right);
  }
}
int btree::maxdistance(bitree root)
{
  int depth;
  return helper(root,depth);
}
int btree::helper(bitree root,int &depth)
{
  if(root==NULL)
   {
    depth=0;return 0;
   }
  else
   {
      int dt,lt;
      int maxleft=helper(root->left,dt);
      int maxright=helper(root->right,lt);
      depth=max(dt,lt)+1;
      return max(maxleft,max(maxright,dt+lt));
  }
}
void btree::print()
{
  createtree(p);
  visitree(p);
  cout<<maxdistance(p);
}
void btree::visitree(bitree root)
{
  if(root)
    {
      cout<<root->data;
      visitree(root->left);
      visitree(root->right);
    }
}
int main()
{
  btree b(15);
  b.print();
  return 0;
}
