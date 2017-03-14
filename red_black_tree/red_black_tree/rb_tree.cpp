#include<iostream>
#define RED 0
#define BLACK 1

struct Node
{
	int key;
	Node *left_child;
	Node *right_child;
	Node *parent;
	int color;
};
using NODE = Node*;
class rb_tree
{
private:
	NODE Root;
public:
	rb_tree();
	void Left_Rotate(NODE z);
	void Right_Rotate(NODE z);
	bool rb_insert(int key);
	bool rb_delete(int key);
	void deplay();
	void visittree(NODE root);
};

rb_tree::rb_tree()
{
	Root = nullptr;
}

void rb_tree::Left_Rotate(NODE z)
{
	NODE y = z->right_child;
	z->right_child = y->left_child;
	if (y->left_child)
		y->left_child->parent = z;
	y->parent = z->parent;
	if (y->parent == nullptr)
	{
		Root = y;
	}
	else
	{
		if (z == z->parent->left_child)
			z->parent->left_child = y;
		else
			z->parent->right_child = y;
	}
	y->left_child = z;
	z->parent = y;
}

void rb_tree::Right_Rotate(NODE z)
{
	NODE y = z->left_child;
	z->left_child = y->right_child;
	if (y->right_child)
		y->right_child->parent = z;
	y->parent = z->parent;
	if (y->parent == nullptr)
	{
		Root = y;
	}
	else
	{
		if (z == z->parent->left_child)
			z->parent->left_child = y;
		else
			z->parent->right_child = y;
	}
	y->right_child = z;
	z->parent = y;
}
bool rb_tree::rb_insert(int key)
{
	if (Root == nullptr)
	{
		NODE newnode = new Node();
		newnode->key = key;
		newnode->color = BLACK;
		newnode->left_child = nullptr;
		newnode->right_child = nullptr;
		newnode->parent = nullptr;
		Root = newnode;
		return true;
	}
	NODE x = Root;
	NODE y = nullptr;
	while (x!= nullptr)
	{
		y = x;
		if (key < x->key)
			x = x->left_child;
		else
			x = x->right_child;
	}
	NODE z = new Node();
	z->key = key;
	z->color = RED;
	z->left_child = nullptr;
	z->right_child = nullptr;
	
	if (key < y->key)
		y->left_child = z;
	else
		y->right_child = z;
	z->parent = y;
	while (z->parent!=nullptr&&z->parent->color == RED)
	{
		if (z->parent == z->parent->parent->left_child)
		{
			/*case1:叔叔节点红*/
			if (z->parent->parent->right_child != nullptr&&z->parent->parent->right_child->color == RED)
			{
				z->parent->color = BLACK;
				z->parent->parent->right_child->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else
			{
				/*case2:是父亲节点右节点*/
				if (z == z->parent->right_child)
				{
					z = z->parent;
					Left_Rotate(z);
				}
				/*case3:是父亲节点左节点*/
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				Right_Rotate(z->parent->parent);
			}
		}
		else
		{
			if (z->parent->parent->left_child != nullptr&&z->parent->parent->left_child->color == RED)
			{
				z->parent->parent->left_child->color = BLACK;
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}
			else
			{
				if (z == z->parent->left_child)
				{
					z = z->parent;
					Right_Rotate(z);
				}
				z->parent->parent->color = RED;
				z->parent->color = BLACK;
				Left_Rotate(z->parent->parent);
			}
		}
	}
	Root->color = BLACK;
	return true;
}

void rb_tree::visittree(NODE root)
{
	if (root != nullptr)
	{
		std::cout << "数据:" << root->key << "颜色" << root->color << std::endl;
		visittree(root->left_child);
		visittree(root->right_child);
	}
}

bool rb_tree::rb_delete(int key)
{
	NODE t = Root;
	NODE z = nullptr;
	NODE y = nullptr;
	NODE x = nullptr;
	while (t != nullptr)
	{
		if (key > t->key)
			t = t->right_child;
		else
			if (key < t->key)
				t = t->left_child;
			else
			{
				z = t;
				break;
			}
	}
	if (z == nullptr)
		return false;
	if (z->left_child == nullptr || z->right_child == nullptr)
		y = z;
	else
	{
		NODE rightnode = z->right_child;
		while (rightnode->left_child != nullptr)
			rightnode = rightnode->left_child;
		y = rightnode;
	}
	if (y->left_child != nullptr)
		x = y->left_child;
	else
		x = y->right_child;
	NODE parent = y->parent;
	if (parent == nullptr)
		Root = x;
	else
		if (y == parent->left_child)
			parent->left_child = x;
		else
			parent->right_child = x;
	if (x)
		x->parent = parent;
	if (y != z)
	{
		z->key = y->key;
	}
	if (y->color == BLACK)
	{
		NODE brother = nullptr;
		while ((!x || x->color == BLACK) && x != Root)
		{
			if (parent->left_child == x)
			{
				brother = parent->right_child;
				/*case:兄弟节点红*/
				if (brother->color == RED)
				{
					parent->color = RED;
					brother->color = BLACK;
					Left_Rotate(parent);
					brother = parent->right_child;
				}
				/*case2:兄弟结点儿子都为黑*/
				if ((!brother->left_child || brother->left_child->color == BLACK) && (!brother->right_child || brother->right_child->color == BLACK))
				{
					brother->color = RED;
					x = parent;
					parent = x->parent;
				}
				else
				{
					/*case3：兄弟结点右儿子为黑*/
					if (!brother->right_child || brother->right_child->color == BLACK)
					{
						brother->left_child->color = BLACK;
						brother->color = RED;
						Right_Rotate(brother);
						brother = parent->right_child;
					}
					/*case4:兄弟节点左儿子为白*/
					brother->color = parent->color;
					parent->color = BLACK;
					brother->right_child->color = BLACK;
					Left_Rotate(parent);
					break;
				}
			}
			else
			{
				brother = parent->left_child;
				if (brother->color == RED)
				{
					brother->color = BLACK;
					parent->color = RED;
					Right_Rotate(parent);
					brother = parent->left_child;
				}
				if ((!brother->left_child || brother->left_child->color == BLACK) && (!brother->right_child || brother->right_child->color == BLACK))
				{
					brother->color = RED;
					x = parent;
					parent = x->parent;
				}
				else
				{
					if (!brother->left_child || brother->left_child->color == BLACK)
					{
						brother->color = RED;
						brother->right_child->color = BLACK;
						Left_Rotate(brother);
						brother = parent->left_child;
					}
					brother->color = parent->color;
					parent->color = BLACK;
					brother->left_child->color = BLACK;
					Right_Rotate(parent);
					break;
				}
			}
		}
		if (x)
			x->color = BLACK;
	}
	return true;
}
void rb_tree::deplay()
{
	visittree(Root);
}
int main()
{
	rb_tree tree;
	tree.rb_insert(11);
	tree.rb_insert(2);
	tree.rb_insert(1);
	tree.rb_insert(7);
	tree.rb_insert(5);
	tree.rb_insert(8);
	tree.rb_insert(14);
	tree.rb_insert(15);
	tree.rb_insert(4);
	tree.rb_insert(65);
	tree.rb_insert(17);
	tree.rb_insert(3);
	tree.deplay();
	std::cout << "----" << std::endl;
	tree.rb_delete(11);
	tree.deplay();
	std::cout << "----" << std::endl;
	tree.rb_delete(7);
	tree.deplay();
	getchar();
	return 0;
}

