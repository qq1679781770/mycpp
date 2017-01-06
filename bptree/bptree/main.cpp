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
	btree.insert(77, 98);
	btree.insert(87, 233);
	btree.insert(323, 87);
	for (int i = 122; i < 140; i++)
		btree.insert(i, i);
	//btree.insert(6, 8);
	btree.print();
	btree.remove(54);
	btree.remove(76);
	std::cout << "********" << std::endl;
	btree.print();
	//std::cout << "****" << btree.search_value(2)<<"****"<<btree.search_value(98);
	system("pause");
	getchar();
}