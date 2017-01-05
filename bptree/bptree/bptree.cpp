#include"bptree.hpp"
/* search the node contain the key to insert or remove*/
template<typename Key,typename Value>
std::shared_ptr<node> bplus_tree<Key, Value>::search_node(std::shared_ptr<node> const& p, Key const& key, bool remove_or_insert)
{
	if (nullptr == root)
	{
		return std::shared_ptr<node>();
	}
/* the node is leaf node*/
	if (dynamic_cast<leafnode<Key>*>(p.get()))
	{
		leafnode<Key>* leaf_node = dynamic_cast<leafnode<Key>*>(p.get());
		if (remove_or_insert)//remove is true
		{
			auto it = std::find(leaf_node->keyItem.begin(), leaf_node->keyItem.end(), key);
			if (it != leaf_node->keyItem.end())
			{
				std::shared_ptr<node>res(leaf_node);
				return res;
			}
			else
			{
				return std::shared_ptr<node>();
			}
		}
		std::shared_ptr<node>res = p;
		return res;
	}
/*the node is internal node then start recursive search*/
	if (dynamic_cast<internalnode<Key>*>(p.get()))
	{
		internalnode<Key>* internal_node = dynamic_cast<internalnode<Key>*>(p.get());
		if (key >= internal_node->keyItem.back())
		{
			return search_node(internal_node->childptr.at(internal_node->childptr.size() - 1), key, remove_or_insert);
		}
		for (size_t i = 0; i < internal_node->keyItem.size(); ++i)
		{
			if (key<internal_node->keyItem.at(i))
				return search_node(internal_node->childptr.at(i), key, remove_or_insert);
		}
	}
/*the node is root node*/
	if (dynamic_cast<rootnode<Key>*>(p.get()))
	{
		rootnode<Key>* root_node = dynamic_cast<rootnode<Key>*>(p.get());
		/*the tree only has a root node*/
		if (dynamic_cast<recordnode<Value>*>(root_node->childptr.at(root_node->childptr.size()-1).get()))
		{
			if (remove_or_insert)
			{
				auto it = std::find(root_node->keyItem.begin(), root_node->keyItem.end(), key);
				if (it != root_node->keyItem.end())
				{
					return std::shared_ptr<node>(root_node);
				}
				return std::shared_ptr<node>();
			}
			std::shared_ptr<node>res=p;
			return res;
		}
		/* recursive search*/
		if (key >= root_node->keyItem.back())
		{
			return search_node(root_node->childptr.at(root_node->childptr.size() - 1), key, remove_or_insert);
		}
		for (size_t i = 0; i < root_node->keyItem.size(); ++i)
		{
			if (key<root_node->keyItem.at(i))
			{
				return search_node(root_node->childptr.at(i), key, remove_or_insert);
			}
		}
	}
}
/*search the record according to key*/
template<typename Key,typename Value>
Value bplus_tree<Key, Value>::search_value(Key const& key)
{
	std::shared_ptr<node>find_node = search_node(root, key, true);
	if (nullptr == find_node) return -1;
	if (dynamic_cast<rootnode<Key>*>(find_node.get()))
	{
		rootnode<Key>* find_root_node = dynamic_cast<rootnode<Key>*>(find_node.get());
		auto it = std::find(find_root_node->keyItem.begin(), find_root_node->keyItem.end(),key);
		size_t index = std::distance(find_root_node->keyItem.begin(), it);
		return dynamic_cast<recordnode<Value>*>(find_root_node->childptr.at(index).get())->value;
	}
	if (dynamic_cast<leafnode<Key>*>(find_node.get()))
	{
		leafnode<Key>* find_leaf_node = dynamic_cast<leafnode<Key>*>(find_node.get());
		auto it = std::find(find_leaf_node->keyItem.begin(), find_leaf_node->keyItem.end(), key);
		size_t index = std::distance(find_leaf_node->keyItem.begin(), it);
		return dynamic_cast<recordnode<Value>*>(find_leaf_node->childptr.at(index).get())->value;
	}
}
/* insert key and record*/
template<typename Key,typename Value>
bool bplus_tree<Key,Value>::insert(const Key& key, const Value& value)
{
	if (nullptr == root)//the tree is empty
	{
		rootnode<Key>* root_node=new rootnode<Key>;
		root_node->keyItem.push_back(key);
		root_node->childptr.push_back(std::make_shared<recordnode<Value>>(value));
		root.reset(root_node);
		return true;
	}
	std::shared_ptr<node>insert_node = search_node(root, key, false);
/* the tree is only has a root*/
	if (dynamic_cast<rootnode<Key>*>(insert_node.get()))
	{
		/*find the index position to insert*/
		rootnode<Key>* insert_root_node = dynamic_cast<rootnode<Key>*>(insert_node.get());
		if (insert_root_node->keyItem.back() <= key)
		{
			insert_root_node->keyItem.push_back(key);
			insert_root_node->childptr.push_back(std::make_shared<recordnode<Value>>(value));
		}
		else
		{
			for (size_t i = 0; i < insert_root_node->keyItem.size(); ++i)
			{
				if (key<insert_root_node->keyItem.at(i))
				{
					insert_root_node->keyItem.insert(insert_root_node->keyItem.begin() + i, key);
					insert_root_node->childptr.insert(insert_root_node->childptr.begin() + i, std::make_shared<recordnode<Value>>(value));
					break;
				}
			}
		}
		if (insert_root_node->keyItem.size() == order)//the numbers of keys is equal to the order of tree ,then split the node
		{
			splite_node(insert_node);
		}
		return true;
	}
/*the leaf node to be inserted*/
	if (dynamic_cast<leafnode<Key>*>(insert_node.get()))
	{
		leafnode<Key>* insert_leaf_node = dynamic_cast<leafnode<Key>*>(insert_node.get());
		auto it=std::find(insert_leaf_node->keyItem.begin(), insert_leaf_node->keyItem.end(), key);
		if (it != insert_leaf_node->keyItem.end())
		{
			return false;
		}
		if (key >= insert_leaf_node->keyItem.back())
		{
			insert_leaf_node->keyItem.push_back(key);
			insert_leaf_node->childptr.push_back(std::make_shared<recordnode<Value>>(value));
		}
		else
		{
			for (size_t i = 0; i < insert_leaf_node->keyItem.size(); ++i)
			{
				if (key < insert_leaf_node->keyItem.at(i))
				{
					insert_leaf_node->keyItem.insert(insert_leaf_node->keyItem.begin() + i, key);
					insert_leaf_node->childptr.insert(insert_leaf_node->childptr.begin() + i, std::make_shared<recordnode<Value>>(value));
					break;
				}
			}
		}
		if (insert_leaf_node->keyItem.size() == order)
		{
			splite_node(insert_node);
		}
		return true;
	}
}
/*insert the key from the splited node to the node's parent and insert the new node from splited node*/
template<typename Key, typename Value>
void bplus_tree<Key, Value>::insert_in_parent(std::shared_ptr<node>& p,const Key& key,std::shared_ptr<node>& child_node)
{
	if (dynamic_cast<rootnode<Key>*>(p.get()))
	{
		rootnode<Key>* insert_root_node = dynamic_cast<rootnode<Key>*>(p.get());
		if (key >= insert_root_node->keyItem.back())
		{
			insert_root_node->keyItem.push_back(key);
			insert_root_node->childptr.push_back(child_node);
		}
		else
		{
			for (size_t i = 0; i < insert_root_node->keyItem.size(); ++i)
			{
				if (key < insert_root_node->keyItem.at(i))
				{
					insert_root_node->keyItem.insert(insert_root_node->keyItem.begin() + i, key);
					insert_root_node->childptr.insert(insert_root_node->childptr.begin() + i+1,child_node);
					break;
				}
			}
		}
		if (dynamic_cast<internalnode<Key>*>(child_node.get()))
		{
			dynamic_cast<internalnode<Key>*>(child_node.get())->parent = p;
		}
		if (dynamic_cast<leafnode<Key>*>(child_node.get()))
		{
			dynamic_cast<leafnode<Key>*>(child_node.get())->parent = p;
		}
		if (insert_root_node->keyItem.size() == order)//the numbers of parents node's keys is equal to order ,then continue split 
		{
			splite_node(p);
		}
		return;
	}
	if (dynamic_cast<internalnode<Key>*>(p.get()))
	{
		internalnode<Key>* insert_internal_node = dynamic_cast<internalnode<Key>*>(p.get());
		if (key >= insert_internal_node->keyItem.back())
		{
			insert_internal_node->keyItem.push_back(key);
			insert_internal_node->childptr.push_back(child_node);
		}
		else
		{
			for (size_t i = 0; i < insert_internal_node->keyItem.size(); ++i)
			{
				if (key < insert_internal_node->keyItem.at(i))
				{
					insert_internal_node->keyItem.insert(insert_internal_node->keyItem.begin() + i, key);
					insert_internal_node->childptr.insert(insert_internal_node->childptr.begin() + i+1,child_node);
					break;
				}
			}
		}
		if (dynamic_cast<internalnode<Key>*>(child_node.get()))
		{
			dynamic_cast<internalnode<Key>*>(child_node.get())->parent = p;
		}
		if (dynamic_cast<leafnode<Key>*>(child_node.get()))
		{
			dynamic_cast<leafnode<Key>*>(child_node.get())->parent = p;
		}
		if (insert_internal_node->keyItem.size() == order)
		{
			splite_node(p);
		}
		return;
	}
}
/* splite the node whose numbers is equal to the order*/
template<typename Key,typename Value>
void bplus_tree<Key, Value>::splite_node(std::shared_ptr<node>& p)
{
/*the node is root node*/
	if(dynamic_cast<rootnode<Key>*>(p.get()))
	{   
		rootnode<Key>* splitenode = dynamic_cast<rootnode<Key>*>(p.get());
		if (dynamic_cast<recordnode<Value>*>(splitenode->childptr.back().get()))
		{
			leafnode<Key>* left_node = new leafnode<Key>;
			leafnode<Key>* right_node = new leafnode<Key>;
			size_t divide_port = order / 2;
			Key divide_key = splitenode->keyItem.at(divide_port);
			for (size_t i = 0; i < divide_port; ++i)
			{
				left_node->keyItem.push_back(splitenode->keyItem.at(i));
				left_node->childptr.push_back(splitenode->childptr.at(i));
			}
			for (size_t i = divide_port; i < order; ++i)
			{
				right_node->keyItem.push_back(splitenode->keyItem.at(i));
				right_node->childptr.push_back(splitenode->childptr.at(i));
			}
			std::shared_ptr<node>new_left_node(left_node);
			std::shared_ptr<node>new_right_node(right_node);
			rootnode<Key>* _root = new rootnode<Key>;
			_root->keyItem.push_back(divide_key);
			dynamic_cast<leafnode<Key>*>(new_left_node.get())->prev = nullptr;
			dynamic_cast<leafnode<Key>*>(new_left_node.get())->next = new_right_node;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->prev = new_left_node;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->next = nullptr;
			_root->childptr.push_back(new_left_node);
			_root->childptr.push_back(new_right_node);
			root.reset(_root);
			dynamic_cast<leafnode<Key>*>(new_left_node.get())->parent = root;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->parent = root;
			return;
		}
		internalnode<Key>* left_node = new internalnode<Key>;
		internalnode<Key>* right_node = new internalnode<Key>;
		size_t divide_port = order/2;
		Key divide_key = splitenode->keyItem.at(divide_port);
		for (size_t i = 0; i < divide_port; ++i)
		{
			left_node->keyItem.push_back(splitenode->keyItem.at(i));
			left_node->childptr.push_back(splitenode->childptr.at(i));
		}
		left_node->childptr.push_back(splitenode->childptr.at(divide_port));
		for (size_t i = divide_port + 1; i < order; ++i)
		{
			right_node->keyItem.push_back(splitenode->keyItem.at(i));
			right_node->childptr.push_back(splitenode->childptr.at(i));
		}
		right_node->childptr.push_back(splitenode->childptr.at(order));
		std::shared_ptr<node>new_left_node(left_node);
		std::shared_ptr<node>new_right_node(right_node);
		for (size_t i = 0; i < left_node->childptr.size(); ++i)
		{
			if (dynamic_cast<internalnode<Key>*>(left_node->childptr.at(i).get()))
			{
				dynamic_cast<internalnode<Key>*>(left_node->childptr.at(i).get())->parent = new_left_node;
			}
			if (dynamic_cast<leafnode<Key>*>(left_node->childptr.at(i).get()))
			{
				dynamic_cast<leafnode<Key>*>(left_node->childptr.at(i).get())->parent = new_left_node;
			}
		}
		for (size_t i = 0; i < right_node->childptr.size(); ++i)
		{
			if (dynamic_cast<internalnode<Key>*>(right_node->childptr.at(i).get()))
			{
				dynamic_cast<internalnode<Key>*>(right_node->childptr.at(i).get())->parent = new_right_node;
			}
			if (dynamic_cast<leafnode<Key>*>(right_node->childptr.at(i).get()))
			{
				dynamic_cast<leafnode<Key>*>(right_node->childptr.at(i).get())->parent = new_right_node;
			}
		}
		dynamic_cast<internalnode<Key>*>(new_left_node.get())->next = new_right_node;
		dynamic_cast<internalnode<Key>*>(new_left_node.get())->prev = nullptr;
		dynamic_cast<internalnode<Key>*>(new_right_node.get())->next = nullptr;
		dynamic_cast<internalnode<Key>*>(new_right_node.get())->prev = new_left_node;
		rootnode<Key>* _root = new rootnode<Key>;
		_root->keyItem.push_back(divide_key);
		_root->childptr.push_back(new_left_node);
		_root->childptr.push_back(new_right_node);
		root.reset(_root);
		dynamic_cast<internalnode<Key>*>(new_left_node.get())->parent = root;
		dynamic_cast<internalnode<Key>*>(new_right_node.get())->parent = root;
		return;
	}
	if (dynamic_cast<leafnode<Key>*>(p.get()))
	{
		leafnode<Key>* splitenode = dynamic_cast<leafnode<Key>*>(p.get());
		std::shared_ptr<node>splite_parent_node = splitenode->parent;
		leafnode<Key>* right_node = new leafnode<Key>;
		size_t divide_port = order/2;
		Key divide_key = splitenode->keyItem.at(divide_port);
		for (size_t i = divide_port; i < order; ++i)
		{
			right_node->keyItem.push_back(splitenode->keyItem.at(i));
			right_node->childptr.push_back(splitenode->childptr.at(i));
		}
		for (size_t i = 0; i < order - divide_port; ++i)
		{
			splitenode->childptr.pop_back();
			splitenode->keyItem.pop_back();
		}
		std::shared_ptr<node>new_right_node(right_node);
		if (nullptr == splitenode->next)
		{
			splitenode->next = new_right_node;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->next = nullptr;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->prev = p;
		}
		else
		{
			dynamic_cast<leafnode<Key>*>(splitenode->next.get())->prev = new_right_node;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->next = splitenode->next;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->prev = p;
			splitenode->next = new_right_node;
		}
		insert_in_parent(splite_parent_node, divide_key, new_right_node);
	}
	if (dynamic_cast<internalnode<Key>*>(p.get()))
	{
		internalnode<Key>* splitenode = dynamic_cast<internalnode<Key>*>(p.get());
		size_t divide_port = order/2;
		Key divide_key = splitenode->keyItem.at(divide_port);
		std::shared_ptr<node>splite_parent_node = splitenode->parent;
		internalnode<Key>* right_node = new internalnode<Key>;
		for (size_t i = divide_port+1; i < order; ++i)
		{
			right_node->keyItem.push_back(splitenode->keyItem.at(i));
			right_node->childptr.push_back(splitenode->childptr.at(i));
		}
		right_node->childptr.push_back(splitenode->childptr.at(order));
		for (size_t i = 0; i < order - divide_port; ++i)
		{
			splitenode->childptr.pop_back();
			splitenode->keyItem.pop_back();
		}
		std::shared_ptr<node>new_right_node(right_node);
		if (nullptr == splitenode->next)
		{
			splitenode->next = new_right_node;
			dynamic_cast<internalnode<Key>*>(new_right_node.get())->next = nullptr;
			dynamic_cast<internalnode<Key>*>(new_right_node.get())->prev = p;
		}
		else
		{
			dynamic_cast<internalnode<Key>*>(splitenode->next.get())->prev = new_right_node;
			dynamic_cast<internalnode<Key>*>(new_right_node.get())->next =splitenode->next;
			dynamic_cast<internalnode<Key>*>(new_right_node.get())->prev = p;
			splitenode->next = new_right_node;
		}
		for (size_t i = 0; i < splitenode->childptr.size(); ++i)
		{
			if (dynamic_cast<internalnode<Key>*>(splitenode->childptr.at(i).get()))
			{
				dynamic_cast<internalnode<Key>*>(splitenode->childptr.at(i).get())->parent = p;
			}
			if (dynamic_cast<leafnode<Key>*>(splitenode->childptr.at(i).get()))
			{
				dynamic_cast<leafnode<Key>*>(splitenode->childptr.at(i).get())->parent = p;
			}
		}
		for (size_t i = 0; i < right_node->childptr.size(); ++i)
		{
			if (dynamic_cast<internalnode<Key>*>(right_node->childptr.at(i).get()))
			{
				dynamic_cast<internalnode<Key>*>(right_node->childptr.at(i).get())->parent = new_right_node;
			}
			if (dynamic_cast<leafnode<Key>*>(right_node->childptr.at(i).get()))
			{
				dynamic_cast<leafnode<Key>*>(right_node->childptr.at(i).get())->parent = new_right_node;
			}
		}
		insert_in_parent(splite_parent_node, divide_key, new_right_node);
	}
}
/*solve the numbers of keys less then order/2*/
template<typename Key,typename Value>
void bplus_tree<Key, Value>::solveunderflow(std::shared_ptr<node>& p)
{
	if (dynamic_cast<leafnode<Key>*>(p.get()))
	{
		leafnode<Key*>_node = dynamic_cast<leafnode<Key>*>(p.get());
		if ((nullptr == _node->prev || dynamic_cast<leafnode<Key>*>(_node->prev.get())!=_node->parent||dynamic_cast<leafnode<Key>*>(_node->prev.get())->keyItem.size() == order / 2) && (nullptr == _node->next || dynamic_cast<leafnode<Key>*>(_node->next.get()) != _node->parent || dynamic_cast<leafnode<Key>*>(_node->next.get())->keyItem.size() == order / 2))
		{
			combine_node(p);
			return;
		}
		borrow_node(p);
		return;
	}
	if (dynamic_cast<internalnode<Key>*>(p.get()))
	{
		internalnode<Key>* _node = dynamic_cast<internalnode<Key>*>(p.get());
		if ((nullptr == _node->prev || dynamic_cast<internalnode<Key>*>(_node->prev.get()) != _node->parent || dynamic_cast<internalnode<Key>*>(_node->prev.get())->keyItem.size() == order / 2) && (nullptr == _node->next || dynamic_cast<internalnode<Key>*>(_node->next.get()) != _node->parent || dynamic_cast<internalnode<Key>*>(_node->next.get())->keyItem.size() == order / 2))
		{
			combine_node(p);
			return;
		}
		borrow_node(p);
		return;
	}
}
/* borrow the key from brother node or father node*/
template<typename Key,typename Value>
void bplus_tree<Key, Value>::borrow_node(std::shared_ptr<node>& p)
{
	if (dynamic_cast<leafnode<Key>*>(p.get()))
	{
		leafnode<Key>* _node = dynamic_cast<leafnode<Key>*>(p.get());
		if (_node->prev&&dynamic_cast<leafnode<Key>*>(_node->prev.get())->parent == _node->parent&&dynamic_cast<leafnode<Key>*>(_node->prev.get())->keyItem.size()>order/2)
		{
			leafnode<Key>* prev_node = dynamic_cast<leafnode<Key>*>(_node->prev.get());
			_node->keyItem.insert(_node->keyItem.begin(), prev_node->keyItem.at(prev_node->keyItem.size() - 1));
			_node->childptr.insert(_node->childptr.begin(), prev_node->childptr.at(prev_node->childptr.size() - 1));
			prev_node->keyItem.pop_back();
			prev_node->childptr.pop_back();
			if (dynamic_cast<internalnode<Key>*>(_node->parent.get()))
			{
				internalnode<Key>* parent_node = dynamic_cast<internalnode<Key>*>(_node->parent.get());
				auto it = std::find(parent_node->childptr.begin(), parent_node->childptr.end(), _node->prev);
				size_t index = std::distance(parent_node->childptr.begin(), it);
				parent_node->keyItem.at(index) = _node->keyItem.at(0);
			}
			if (dynamic_cast<rootnode<Key>*>(_node->parent.get()))
			{
				rootnode<Key>* parent_node = dynamic_cast<rootnode<Key>*>(_node->parent.get());
				auto it = std::find(parent_node->childptr.begin(), parent_node->childptr.end(), _node->prev);
				size_t index = std::distance(parent_node->childptr.begin(), it);
				parent_node->keyItem.at(index) = _node->keyItem.at(0);
			}
			return;
		}
		if (_node->next&&dynamic_cast<leafnode<Key>*>(_node->next.get())->parent == _node->parent&&dynamic_cast<leafnode<Key>*>(_node->next.get())->keyItem.size() > order / 2)
		{
			leafnode<Key>* next_node = dynamic_cast<leafnode<Key>*>(_node->next.get());
			_node->keyItem.push_back(next_node->keyItem.at(0));
			_node->childptr.push_back(next_node->childptr.at(0));
			next_node->keyItem.erase(next_node->keyItem.begin());
			next_node->childptr.erase(next_node->childptr.begin());
			if (dynamic_cast<internalnode<Key>*>(_node->parent.get()))
			{
				internalnode<Key>* parent_node = dynamic_cast<internalnode<Key>*>(_node->parent.get());
				auto it = std::find(parent_node->childptr.begin(), parent_node->childptr.end(), p);
				size_t index = std::distance(parent_node->childptr.begin(), it);
				parent_node->keyItem.at(index) = next_node->keyItem.at(0);
			}
			if (dynamic_cast<rootnode<Key>*>(_node->parent.get()))
			{
				rootnode<Key>* parent_node = dynamic_cast<rootnode<Key>*>(_node->parent.get());
				auto it = std::find(parent_node->childptr.begin(), parent_node->childptr.end(), p);
				size_t index = std::distance(parent_node->childptr.begin(), it);
				parent_node->keyItem.at(index) = next_node->keyItem.at(0);
			}
			return;
		}
	}
	if (dynamic_cast<internalnode<Key>*>(p.get()))
	{
		internalnode<Key>* _node = dynamic_cast<internalnode<Key>*>(p.get());
		if (_node->prev&&dynamic_cast<internalnode<Key>*>(_node->prev.get())->parent == _node->parent&&dynamic_cast<internalnode<Key>*>(_node->prev.get())->keyItem.size() > order / 2)
		{
			internalnode<Key>* prev_node = dynamic_cast<internalnode<Key>*>(_node->prev.get());
			_node->childptr.insert(_node->childptr.begin(), prev_node->childptr.at(prev_node->childptr.size() - 1));
			prev_node->childptr.pop_back();
			if (dynamic_cast<internalnode<Key>*>(_node->childptr.at(0).get()))
			{
				dynamic_cast<internalnode<Key>*>(_node->childptr.at(0).get())->parent = p;
			}
			if (dynamic_cast<leafnode<Key>*>(_node->childptr.at(0).get()))
			{
				dynamic_cast<leafnode<Key>*>(_node->childptr.at(0).get())->parent = p;
			}
			if (dynamic_cast<rootnode<Key>*>(_node->parent.get()))
			{
				rootnode<Key>* parent_node = dynamic_cast<rootnode<Key>*>(_node->parent.get());
				auto it = std::find(parent_node->childptr.begin(), parent_node->childptr.end(), _node->prev);
				size_t index = std::distance(parent_node->childptr.begin(), it);
				_node->keyItem.insert(_node->keyItem.begin(), parent_node->keyItem.at(index));
				parent_node->keyItem.at(index) = prev_node->keyItem.at(prev_node->keyItem.size() - 1);
				prev_node->keyItem.pop_back();
			}
			if (dynamic_cast<internalnode<Key>*>(_node->parent.get()))
			{
				internalnode<Key>* parent_node = dynamic_cast<internalnode<Key>*>(_node->parent.get());
				auto it = std::find(parent_node->childptr.begin(), parent_node->childptr.end(), _node->prev);
				size_t index = std::distance(parent_node->childptr.begin(), it);
				_node->keyItem.insert(_node->keyItem.begin(), parent_node->keyItem.at(index));
				parent_node->keyItem.at(index) = prev_node->keyItem.at(prev_node->keyItem.size() - 1);
				prev_node->keyItem.pop_back();
			}
			return;
		}
		if (_node->next&&dynamic_cast<internalnode<Key>*>(_node->next.get())->parent == _node->parent&&dynamic_cast<internalnode<Key>*>(_node->next.get())->keyItem.size() > order / 2)
		{
			internalnode<Key>* next_node = dynamic_cast<internalnode<Key>*>(_node->next.get());
			_node->childptr.push_back(next_node->childptr.at(0));
			next_node->childptr.erase(next_node->childptr.begin());
			next_node->keyItem.erase(next_node->keyItem.begin());
			if (dynamic_cast<internalnode<Key>*>(_node->childptr.at(_node->childptr.size() - 1).get()))
			{
				dynamic_cast<internalnode<Key>*>(_node->childptr.at(_node->childptr.size() - 1).get())->parent = p;
			}
			if (dynamic_cast<leafnode<Key>*>(_node->childptr.at(_node->childptr.size() - 1).get()))
			{
				dynamic_cast<leafnode<Key>*>(_node->childptr.at(_node->childptr.size() - 1).get())->parent = p;
			}
			if (dynamic_cast<rootnode<Key>*>(_node->parent.get()))
			{
				rootnode<Key>* parent_node = dynamic_cast<rootnode<Key>*>(_node->parent.get());
				auto it = std::find(parent_node->childptr.begin(), parent_node->childptr.end(), p);
				size_t index = std::distance(parent_node->childptr.begin(), it);
				_node->keyItem.push_back(parent_node->keyItem.at(index));
				parent_node->keyItem.at(index) = next_node->keyItem.at(0);
			}
			if (dynamic_cast<internalnode<Key>*>(_node->parent.get()))
			{
				internalnode<Key>* parent_node = dynamic_cast<internalnode<Key>*>(_node->parent.get());
				auto it = std::find(parent_node->childptr.begin(), parent_node->childptr.end(), p);
				size_t index = std::distance(parent_node->childptr.begin(), it);
				_node->keyItem.push_back(parent_node->keyItem.at(index));
				parent_node->keyItem.at(index) = next_node->keyItem.at(0);
			}
			return;
		}
	}
}
/*combine the node with brother node*/
template<typename Key,typename Value>
void bplus_tree<Key, Value>::combine_node(std::shared_ptr<node>& p)
{

}
/*remove one key and record*/
template<typename Key,typename Value>
bool bplus_tree<Key, Value>::remove(Key const& key)
{
	std::shared_ptr<node>remove_node = search_node(root, key, true);
	if (nullptr == remove_node) return false;
	if (dynamic_cast<rootnode<Key>*>(remove_node.get()))
	{
		rootnode<Key>* remove_root_node = dynamic_cast<rootnode<Key>*>(remove_node.get());
		auto it = std::find(remove_root_node->keyItem.begin(), remove_root_node->keyItem.end(), key);
		size_t index = std::distance(remove_root_node->keyItem.begin(), it);
		remove_root_node->keyItem.erase(it);
		remove_root_node->childptr.erase(remove_root_node->childptr.begin() + index);
		return true;
	}
	if (dynamic_cast<leafnode<Key>*>(remove_node.get()))
	{
		leafnode<Key>* remove_leaf_node = dynamic_cast<leafnode<Key>*>(remove_node.get());
		auto it = std::find(remove_leaf_node->keyItem.begin(), remove_leaf_node->keyItem.end(), key);
		size_t index = std::distance(remove_leaf_node->keyItem.begin(), it);
		remove_leaf_node->keyItem.erase(it);
		remove_leaf_node->childptr.erase(remove_leaf_node->childptr.begin() + index);
		if (remove_leaf_node->keyItem.size() < order / 2)
		{
			solveunderflow(remove_node);
		}
		return true;
	}
}
/*inorder traversal*/
template<typename Key,typename Value>
void bplus_tree<Key, Value>::visit(std::shared_ptr<node>const& _node)
{
	if (dynamic_cast<rootnode<Key>*>(_node.get()))
	{
		std::cout << std::endl;
		rootnode<Key>* displaynode = dynamic_cast<rootnode<Key>*>(_node.get());
		for (size_t i = 0; i < displaynode->keyItem.size(); ++i)
		{
			std::cout << displaynode->keyItem.at(i)<<"----";
		}
		for (size_t i = 0; i < displaynode->childptr.size(); ++i)
		{
			visit(displaynode->childptr.at(i));
		}
	}
	if (dynamic_cast<internalnode<Key>*>(_node.get()))
	{
		std::cout << std::endl;
		internalnode<Key>* displaynode = dynamic_cast<internalnode<Key>*>(_node.get());
		for (size_t i = 0; i < displaynode->keyItem.size(); ++i)
		{
			std::cout << displaynode->keyItem.at(i) << "----";
		}
		for (size_t i = 0; i < displaynode->childptr.size(); ++i)
		{
			visit(displaynode->childptr.at(i));
		}
	}
	if (dynamic_cast<leafnode<Key>*>(_node.get()))
	{
		std::cout << std::endl;
		leafnode<Key>* displaynode = dynamic_cast<leafnode<Key>*>(_node.get());
		for (size_t i = 0; i < displaynode->keyItem.size(); ++i)
		{
			std::cout << displaynode->keyItem.at(i) << "----";
		}
		for (size_t i = 0; i < displaynode->childptr.size(); ++i)
		{
			visit(displaynode->childptr.at(i));
		}
	}
	return;
}

