#include"btree.hpp"
#include<iostream>
#include<algorithm>
template<typename T>
BTree<T>::BTree(int m)
{
    order=m;
    Root=nullptr;
    poot=nullptr;
    qoot=nullptr;
}
template<typename T>
bool BTree<T>::insert( const T& key)
{
    /*根节点为空*/
    if(nullptr==Root)
    {
        Root=new BTNode<T>;
        Root->key.push_back(key);
        Root->child.push_back(nullptr);
        return true;
    }
    bool status_search=search(Root,key);//查找关键词
    if(status_search==true)
        return false;
    if(status_search==false)
    {
        poot->child.push_back(nullptr);
        if(key>poot->key.back())
            poot->key.push_back(key);
        else
        {
            int temp;
            for(int i=0;i<poot->key.size();++i)
                if(key<poot->key.at(i))
                {
                    temp=i;
                    break;
                }
            poot->key.resize(poot->key.size()+1);
            for(int i=poot->key.size()-1;i>temp;i--)
                poot->key.at(i)=poot->key.at(i-1);
            poot->key.at(temp)=key;//插入key
        }
        if(poot->key.size()==order)
            sloveoverflow();
        return true;
    }
}
template<typename T>
bool BTree<T>::search(BTNode<T>*& root, const T& key)
{
    if(nullptr==root)
        return false;
    for(int i=0;i<root->key.size();++i)
        if(key==root->key.at(i))
        {
            qoot=root;//qoot等于要删除的节点
            return true;
        }
    if(nullptr==root->child.back())
    {
        poot=root;//poot等于插入的叶子节点
        return false;
   }
    if(key>root->key.back())
        return search(root->child.at(root->child.size()-1),key);
    else
    {
        for(int i=0;i<root->key.size();++i)
            if(key<root->key.at(i))
                return search(root->child.at(i),key);
    }
}
template<typename T>
void BTree<T>::sloveoverflow()
{
    BTNode<T>* leaftNode=new BTNode<T>;
   leaftNode->child.pop_back();
    BTNode<T>* rightNode=new BTNode<T>;
   rightNode->child.pop_back();
    BTNode<T>*  parentNode=new BTNode<T>;
    parentNode->child.pop_back();
    //
     T divide_key;
     int divide_number=(order+1)/2;
     divide_key=poot->key.at(divide_number-1);
     //左半部分
     for(int i=0;i<divide_number-1;++i)
     {
         leaftNode->key.push_back(poot->key.at(i));
         leaftNode->child.push_back(poot->child.at(i));
     }
     leaftNode->child.push_back(poot->child.at(divide_number-1));
     for(int i=0;i<leaftNode->child.size();++i)
         if(leaftNode->child.at(i))  leaftNode->child.at(i)->parent=leaftNode;
     //右半部分
     for(int i=divide_number;i<poot->key.size();++i)
     {
         rightNode->key.push_back(poot->key.at(i));
         rightNode->child.push_back(poot->child.at(i));
     }
     rightNode->child.push_back(poot->child.at(poot->key.size()));
     for(int i=0;i<rightNode->child.size();++i)
         if(rightNode->child.at(i)) rightNode->child.at(i)->parent=rightNode;
     //父节点为nullptr*/
     if(nullptr==poot->parent)
     {
         parentNode->key.push_back(divide_key);
         parentNode->child.push_back(leaftNode);
         parentNode->child.push_back(rightNode);
         Root=parentNode;
         leaftNode->parent=Root;
         rightNode->parent=Root;
         poot=nullptr;
         return ;
     }
     else
  {
         parentNode=poot->parent;
     //分离关键词大于父节点最大值
     if(divide_key>parentNode->key.back())
     {

         parentNode->key.push_back(divide_key);
         parentNode->child.pop_back();
         parentNode->child.push_back(leaftNode);
         parentNode->child.push_back(rightNode);
         leaftNode->parent=parentNode;
         rightNode->parent=parentNode;
     }
     //
     else
     {
         int temp;
         for(int i=0;i<parentNode->key.size();++i)
         {
             if(divide_key<parentNode->key.at(i))
             {
                 temp=i;
                 break;
             }
         }
         parentNode->key.resize(parentNode->key.size()+1);
         for(int i=parentNode->key.size();i>temp;i--)
             parentNode->key.at(i)=parentNode->key.at(i-1);
         parentNode->key.at(temp)=divide_key;
         //
         parentNode->child.resize(parentNode->child.size()+1);
         for(int i=parentNode->child.size();i>temp+1;i--)
             parentNode->child.at(i)=parentNode->child.at(i-1);
         parentNode->child.at(temp+1)=rightNode;
         parentNode->child.at(temp)=leaftNode;
         //
         leaftNode->parent=parentNode;
         rightNode->parent=parentNode;
     }
}
   if(parentNode->key.size()<order)
   {
       poot=nullptr;
       return ;
   }
   if(parentNode->key.size()==order)
   {
       poot=parentNode;
       sloveoverflow();//递归解决超出树度数
   }
}
//删除元素
template<typename T>
bool BTree<T>::remove(const T &key)
{
    bool status_search=search(Root,key);
    if(!status_search)
        return false;
    int temp;
    //只有根节点
    if((qoot==Root)&&(qoot->child.back()==nullptr))
    {
         std::remove(Root->key.begin(),Root->key.end(),key);
         Root->key.resize(Root->key.size()-1);
         Root->child.pop_back();
        if(Root->key.empty())
        {
            Root=nullptr;
        }
        return true;
    }
    //如果是内节点,删除元素,不断向下取孩子节点的元素直到叶子节点,并删除第一个元素
    if(qoot->child.back())
    {
        for(int i=0;i<qoot->key.size();++i)
        {
            if(qoot->key.at(i)==key)
            {
                temp=i;
                break;
            }
        }
       qoot->key.at(temp)=qoot->child.at(temp+1)->key.at(0);
       qoot=qoot->child.at(temp+1);
       while(qoot->child.back())
       {
        qoot->key.at(0)=qoot->child.at(1)->key.at(0);
        qoot=qoot->child.at(1);
       }
       qoot->key.erase(qoot->key.begin());
       qoot->child.pop_back();
    }
    //如果是叶子节点,直接删除
    else
    {
        std::remove(qoot->key.begin(),qoot->key.end(),key);
        qoot->key.resize(qoot->key.size()-1);
        qoot->child.pop_back();
    }
    //关键词小于下限
    if(qoot->key.size()<(order-1)/2)
        sloveunderflow();
    qoot=nullptr;
    return true;
}
template<typename T>
void BTree<T>::sloveunderflow()
{
    if(qoot==Root) return;
    BTNode<T>* parentNode=qoot->parent;
    int firsttemp=-1;
    int lasttemp=parentNode->child.size();
    int  element=qoot->key.front();
    //父节点的第一个子节点
   if(element<qoot->parent->key.front())
      {
          lasttemp=1;
          //兄弟节点有多余关键词
          if(parentNode->child.at(lasttemp)->key.size()>(order-1)/2)
          {
              qoot->key.push_back(parentNode->key.front());
              qoot->child.push_back(parentNode->child.at(lasttemp)->child.front());
              if(qoot->child.back())
                  qoot->child.at(qoot->child.size()-1)->parent=qoot;
              parentNode->key.at(0)=parentNode->child.at(lasttemp)->key.front();
              parentNode->child.at(lasttemp)->child.erase(parentNode->child.at(lasttemp)->child.begin());
              parentNode->child.at(lasttemp)->key.erase(parentNode->child.at(lasttemp)->key.begin());
              qoot=nullptr;
              return;
          }
          //兄弟节点刚脱贫
          else
          {
              qoot->key.push_back(parentNode->key.front());
              qoot->key.insert(qoot->key.end(),parentNode->child.at(lasttemp)->key.begin(),parentNode->child.at(lasttemp)->key.end());
              qoot->child.insert(qoot->child.end(),parentNode->child.at(lasttemp)->child.begin(),parentNode->child.at(lasttemp)->child.end());
              parentNode->key.erase(parentNode->key.begin());
              parentNode->child.erase(parentNode->child.begin()+1);
              for(int i=0;i<qoot->child.size();++i)
                   if(qoot->child.at(i)) qoot->child.at(i)->parent=qoot;
              if(parentNode==Root&&Root->key.empty())
              {
                  Root=qoot;
                  qoot=nullptr;
                  return;
              }
              if(parentNode->key.size()<(order-1)/2)
              {
                  qoot=parentNode;
                  return sloveunderflow();
              }
              qoot=nullptr;
              return;
          }
       }
   //节点是父节点的最后一个节点
   if(element>qoot->parent->key.back())
   {
       firsttemp=parentNode->key.size()-1;

       if(parentNode->child.at(firsttemp)->key.size()>(order-1)/2)
       {
           qoot->key.insert(qoot->key.begin(),parentNode->key.back());
           qoot->child.insert(qoot->child.begin(),parentNode->child.at(firsttemp)->child.back());
           parentNode->key.at(firsttemp)=parentNode->child.at(firsttemp)->key.back();
           if(qoot->child.front())
               qoot->child.at(0)->parent=qoot;
           parentNode->child.at(firsttemp)->key.erase(parentNode->child.at(firsttemp)->key.end()-1);
           parentNode->child.at(firsttemp)->child.erase(parentNode->child.at(firsttemp)->child.end()-1);
           qoot=nullptr;
           return;
       }
       else
       {
           qoot->key.insert(qoot->key.begin(),parentNode->key.back());
           qoot->key.insert(qoot->key.begin(),parentNode->child.at(firsttemp)->key.begin(),parentNode->child.at(firsttemp)->key.end());
           qoot->child.insert(qoot->child.begin(),parentNode->child.at(firsttemp)->child.begin(),parentNode->child.at(firsttemp)->child.end());
           for(int i=0;i<qoot->child.size();++i)
               if(qoot->child.at(i))  qoot->child.at(i)->parent=qoot;
           parentNode->key.erase(parentNode->key.end()-1);
           parentNode->child.erase(parentNode->child.end()-2);
           if(parentNode==Root&&Root->key.empty())
           {
               Root=qoot;
               qoot=nullptr;
               return;
           }
           if(parentNode->key.size()<(order-1)/2)
           {
               qoot=parentNode;
               return sloveunderflow();
           }
           qoot=nullptr;
           return;
       }
   }
   for(int i=0;i<qoot->parent->key.size()-1;++i)
       if(element>qoot->parent->key.at(i)&&element<qoot->parent->key.at(i+1))
       {
           firsttemp=i;
           lasttemp=i+2;
       }
   //左兄弟有多余关键词
   if(parentNode->child.at(firsttemp)->key.size()>(order-1)/2)
   {
       qoot->key.insert(qoot->key.begin(),parentNode->key.at(firsttemp));
       qoot->child.insert(qoot->child.begin(),parentNode->child.at(firsttemp)->child.back());
       if(qoot->child.front())
           qoot->child.at(0)->parent=qoot;
       parentNode->key.at(firsttemp)=parentNode->child.at(firsttemp)->key.back();
       parentNode->child.at(firsttemp)->key.erase(parentNode->child.at(firsttemp)->key.end()-1);
       parentNode->child.at(firsttemp)->child.erase(parentNode->child.at(firsttemp)->child.end()-1);
       qoot=nullptr;
       return;
   }
   //右兄弟有多余关键词
   if(parentNode->child.at(lasttemp)->key.size()>(order-1)/2)
   {
       qoot->key.push_back(parentNode->key.at(lasttemp-1));
       qoot->child.push_back(parentNode->child.at(lasttemp)->child.front());
       parentNode->key.at(lasttemp-1)=parentNode->child.at(lasttemp)->key.front();
       if(qoot->child.back())
           qoot->child.at(qoot->child.size()-1)->parent=qoot;
       parentNode->child.at(lasttemp)->child.erase(parentNode->child.at(lasttemp)->child.begin());
       parentNode->child.at(lasttemp)->key.erase(parentNode->child.at(lasttemp)->key.begin());
       qoot=nullptr;
       return;
   }
   //左右兄弟都刚脱贫
   if(parentNode->child.at(firsttemp)->key.size()==(order-1)/2&&parentNode->child.at(lasttemp)->key.size()==(order-1)/2)
   {
        qoot->key.push_back(parentNode->key.at(lasttemp-1));
        qoot->key.insert(qoot->key.end(),parentNode->child.at(lasttemp)->key.begin(),parentNode->child.at(lasttemp)->key.end());
        qoot->child.insert(qoot->child.end(),parentNode->child.at(lasttemp)->child.begin(),parentNode->child.at(lasttemp)->child.end());
        for(int i=0;i<qoot->child.size();++i)
            if(qoot->child.at(i))
                qoot->child.at(i)->parent=qoot;
        parentNode->key.erase(parentNode->key.begin()+lasttemp-1);
        parentNode->child.erase(parentNode->child.begin()+lasttemp);
        if(parentNode==Root&&Root->key.empty())
        {
            Root=qoot;
            qoot=nullptr;
            return;
        }
        if(parentNode->key.size()<(order-1)/2)
        {
            qoot=parentNode;
            return sloveunderflow();
        }
        qoot=nullptr;
        return;
   }
}

//输出b树
template<typename T>
void BTree<T>::display()
{
    std::queue<BTNode<T>* >display_queue;
    if(Root)
    {
        display_queue.push(Root);
        while(!display_queue.empty())
        {
            BTNode<T>* d=display_queue.front();
            for(int i=0;i<d->key.size();++i)
                std::cout<<d->key.at(i)<<"---";
            std::cout<<std::endl;
            if(d->child.back())
            {
                for(int i=0;i<d->child.size();++i)
                    display_queue.push(d->child.at(i));
            }
            display_queue.pop();
        }
    }
}
