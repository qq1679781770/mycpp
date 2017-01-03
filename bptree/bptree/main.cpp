#include"bptree.cpp"
int main()
{
	bplus_tree<int, int>btree(5);
	btree.insert(2, 1);
	
	btree.insert(54, 23);
	btree.insert(23, 87);
	btree.insert(53, 32);
	btree.insert(76, 98);
	btree.insert(12, 89);
	btree.insert(98, 89);
	btree.insert(14, 89);
	btree.insert(76, 98);
	btree.insert(87, 233);
	btree.insert(323, 87);
	for (int i = 122; i < 135; i++)
		btree.insert(i, i);
	
}