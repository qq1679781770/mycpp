#include <iostream>
using namespace std;
struct  btree
{
    int data;
    btree * left;
    btree * right;
};
class Tree
{
private:
    int  *number;
    int length;
    btree *tree;
public:
    void createtree(btree* &root);
    bool insertdata(btree* &root,int data);
    void visittree(btree* root);
    bool searchdata(btree *root,int data);
    void deletedata(btree *&root,int data);
    void print();
    Tree(int n);
    ~Tree(){delete number;}

};
Tree::Tree(int n)
{
    tree=NULL;
    length=n;
    number=new int[length];
    cout<<"请输入"<<length<<"个数字:";
    for(int i=0;i<length;i++)
    cin>>number[i];
}
void Tree::createtree(btree *&root)
{
    root=new btree;
    root->data=number[0];
    root->left=NULL;
    root->right=NULL;
    for(int i=1;i<length;i++)
        insertdata(root,number[i]);
}
bool Tree::insertdata(btree *&root, int data)
{
    if(root==NULL)
    {
        root=new btree;
        root->data=data;
        root->left=NULL;
        root->right=NULL;
        return true;
    }
    else
    {
        if(data>root->data) insertdata(root->right,data);
        else insertdata(root->left,data);
    }
}
void Tree::visittree(btree *root)
{
    if(root!=NULL)
    {
        cout<<root->data<<" ";
        visittree(root->left);
        visittree(root->right);
    }
}
bool Tree::searchdata(btree *root, int data)
{
    if(root!=NULL)
    {
        if(root->data==data)
            return  true;
        if(root->data<data)
            searchdata(root->right,data);
        if(root->data>data)
            searchdata(root->left,data);
    }
}
void  Tree::deletedata(btree *&root, int data)
{
    btree *p,*parent,*s;
    p=root;
    if(root==NULL) return ;
    if(p->data==data)
    {
        if(!p->left&&!p->right)
        {
            root=NULL;
            delete root;
        }
        else
        {
            if(!p->left)
                root=p->right;
            if(!p->right)
                root=p->left;
            if(p->left&&p->right)
            {
                s=p->right;
                if(!s->left)
                    s->left=p->left;
               else
                {
                    while(s->left)
                {
                    parent=s;
                    s=s->left;
                }
                parent->left=s->right;
                s->right=p->right;
                s->left=p->left;
                }
                root=s;
            }
        }
    }
    else
        if(p->data<data) deletedata(root->right,data);
      else  deletedata(root->left,data);
}
void Tree::print()
{
    createtree(tree);
    visittree(tree);
    deletedata(tree,number[7]);
    visittree(tree);
}


int main()
{
    Tree a(10);
    a.print();
    return 0;
}

