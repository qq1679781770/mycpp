#include<iostream>
#define RED 0
#define BLACK 1

struct Node
{
    int key;
    Node *left_child;
    Node *right_child;
    NOde *parent;
    int color;
};
using NODE=Node*;

class rb_tree
{
private:
    NODE Root;
public:
    rb_tree();
    bool RB_insert(NODE& root,int key);
    bool RB_delete(NODE& root,int key);
    void RB_insert_fixup(NODE& T,NODE& z);
    void RB_delete_fixup(NODE& T,NODE& z);
};
   
rb_tree::rb_tree()
{
    Root=nullptr;

}
bool rb_tree::RB_insert(NODE& root,int key)
{
    if(Root==nullptr)
    {
        NODE newnode=new Node();
    }
}