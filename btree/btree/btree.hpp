#ifndef BTREE_HPP
#define BTREE_HPP
#include<queue>
#include<vector>
template <typename T>
class BTNode
{
public:
    BTNode* parent;
    std::vector<T>key;
    std::vector<BTNode<T>*> child;
public:
    BTNode():parent(nullptr){child.push_back(nullptr);}
};
template<typename T>
class BTree
{
private:
    int order;
     BTNode<T>*  Root;//根节点
     BTNode<T>* poot;//插入元素节点
     BTNode<T>* qoot;//删除元素节点
public:
    BTree(int m);
    bool  insert(const  T& key);
    bool remove(const T &key);
    bool search(BTNode<T>*& root,const T& key);//搜索插入元素或删除元素;
    void sloveoverflow();//解决节点关键词超出度数;
    void sloveunderflow();//解决关键词不足问题;
    void display();
};
#endif // BTREE_HPP
