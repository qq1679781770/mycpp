#include <iostream>
#include"btree.hpp"
#include"btree.cpp"
int main()
{
   BTree<int>  tree(5);
  tree.insert(8);
  tree.insert(9);
   tree.insert(6);
   tree.insert(98);
   tree.display();
   tree.remove(6);
   tree.display();

  /* tree.insert(76);
   tree.insert(81);
  tree.insert(92);
  tree.insert(63);
   tree.insert(938);
   tree.insert(726);
   tree.insert(99);
   tree.insert(100);
   tree.insert(101);
   tree.insert(102);
   tree.insert(103);
   tree.insert(104);
   tree.insert(105);
   tree.insert(727);
   tree.insert(728);
   tree.insert(729);
   tree.insert(730);
   tree.insert(731);
   tree.insert(732);
   tree.insert(733);
   tree.insert(939);
   tree.insert(940);
   tree.insert(98);
   tree.display();
   tree.remove(98);
   tree.display();
   tree.remove(940);
   tree.display();
   tree.remove(105);
   tree.display();*/
    std::cout << "Hello World!" << std::endl;
    return 0;
}
