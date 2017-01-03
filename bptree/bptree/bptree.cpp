#include"bptree.hpp"
template<typename Key,typename Value>
std::shared_ptr<node> bplus_tree<Key, Value>::search_node(std::shared_ptr<node> const& p, Key const& key, bool remove_or_insert)
{
	if (nullptr == root)
	{
		return std::shared_ptr<node>();
	}
	if (dynamic_cast<leafnode<Key>*>(p.get()))
	{
		leafnode<Key>* leaf_node = dynamic_cast<leafnode<Key>*>(p.get());
		if (remove_or_insert)
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
	if (dynamic_cast<internalnode<Key>*>(p.get()))
	{
		internalnode<Key>* internal_node = dynamic_cast<internalnode<Key>*>(p.get());
		if (key >= internal_node->keyItem.back())
		{
			return search_node(internal_node->childptr.at(internal_node->childptr.size() - 1), key, remove_or_insert);
		}
		for (size_t i = 0; i < internal_node->keyItem.size(); ++i)
		{
			if (internal_node->keyItem.at(i) < key)
				return search_node(internal_node->childptr.at(i), key, remove_or_insert);
		}
	}
	if (dynamic_cast<rootnode<Key>*>(p.get()))
	{
		rootnode<Key>* root_node = dynamic_cast<rootnode<Key>*>(p.get());
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
template<typename Key,typename Value>
bool bplus_tree<Key,Value>::insert(const Key& key, const Value& value)
{
	if (nullptr == root)
	{
		rootnode<Key>* root_node=new rootnode<Key>;
		root_node->keyItem.push_back(key);
		root_node->childptr.push_back(std::make_shared<recordnode<Value>>(value));
		root.reset(root_node);
		return true;
	}
	std::shared_ptr<node>insert_node = search_node(root, key, false);
	if (dynamic_cast<rootnode<Key>*>(insert_node.get()))
	{
		rootnode<Key>* insert_root_node = dynamic_cast<rootnode<Key>*>(insert_node.get());
		if (insert_root_node->keyItem.back() <= key)
		{
			insert_root_node->keyItem.push_back(key);
			insert_root_node->childptr.push_back(std::make_shared<recordnode<Value>>(value));
			//insert_node.reset(insert_root_node);
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
		if (insert_root_node->keyItem.size() == order)
		{
			splite_node(insert_node);
		}
		return true;
	}
	if (dynamic_cast<leafnode<Key>*>(insert_node.get()))
	{
		leafnode<Key>* insert_leaf_node = dynamic_cast<leafnode<Key>*>(insert_node.get());
		//for (size_t i = 0; i < insert_leaf_node->keyItem.size; ++i)  std::cout << insert_leaf_node->keyItem.at(i) << std::endl;
		if (key >= insert_leaf_node->keyItem.back())
		{
			insert_leaf_node->keyItem.push_back(key);
			insert_leaf_node->childptr.push_back(std::make_shared<recordnode<Value>>(value));
			//insert_node.reset(insert_leaf_node);
		}
		else
		{
			for (size_t i = 0; i < insert_leaf_node->keyItem.size(); ++i)
			{
				if (key < insert_leaf_node->keyItem.at(i))
				{
					insert_leaf_node->keyItem.insert(insert_leaf_node->keyItem.begin() + i, key);
					insert_leaf_node->childptr.insert(insert_leaf_node->childptr.begin() + i, std::make_shared<recordnode<Value>>(value));
					//insert_node.reset(insert_leaf_node);
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
					insert_root_node->childptr.insert(insert_root_node->childptr.begin() + i, child_node);
					break;
				}
			}
		}
		if (insert_root_node->keyItem.size() == order)
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
					insert_internal_node->childptr.insert(insert_internal_node->childptr.begin() + i, child_node);
					break;
				}
			}
		}
		if (insert_internal_node->keyItem.size() == order)
		{
			splite_node(p);
		}
		return;
	}
}
template<typename Key,typename Value>
void bplus_tree<Key, Value>::splite_node(std::shared_ptr<node>& p)
{
	if(dynamic_cast<rootnode<Key>*>(p.get()))
	{   
		rootnode<Key>* splitenode = dynamic_cast<rootnode<Key>*>(p.get());
		if (dynamic_cast<recordnode<Value>*>(splitenode->childptr.back().get()))
		{
			leafnode<Key>* left_node = new leafnode<Key>;
			leafnode<Key>* right_node = new leafnode<Key>;
			size_t divide_port = order / 2;
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
			_root->keyItem.push_back(splitenode->keyItem.at(divide_port));
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
		size_t divide_port = order / 2;
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
		if (nullptr == splitenode->next)
		{
			dynamic_cast<leafnode<Key>*>(new_left_node.get())->next = new_right_node;
			dynamic_cast<leafnode<Key>*>(new_left_node.get())->prev = splitenode->prev;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->next = nullptr;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->prev = new_left_node;
		}
		else
		{
			dynamic_cast<leafnode<Key>*>(new_left_node.get())->next = new_right_node;
			dynamic_cast<leafnode<Key>*>(new_left_node.get())->prev = splitenode->prev;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->next = splitenode->next;
			dynamic_cast<leafnode<Key>*>(new_right_node.get())->prev = new_left_node;
		}
		dynamic_cast<leafnode<Key>*>(new_left_node.get())->parent = splite_parent_node;
		dynamic_cast<leafnode<Key>*>(new_right_node.get())->parent = splite_parent_node;
		p = new_left_node;
		insert_in_parent(splite_parent_node, divide_key, new_right_node);
	}
	if (dynamic_cast<internalnode<Key>*>(p.get()))
	{
		internalnode<Key>* splitenode = dynamic_cast<internalnode<Key>*>(p.get());
		size_t divide_port = order / 2;
		Key divide_key = splitenode->keyItem.at(divide_port);
		std::shared_ptr<node>splite_parent_node = splitenode->parent;
		internalnode<Key>* left_node = new internalnode<Key>;
		internalnode<Key>* right_node = new internalnode<Key>;
		for (size_t i = 0; i < divide_port; ++i)
		{
			left_node->keyItem.push_back(splitenode->keyItem.at(i));
			left_node->childptr.push_back(splitenode->childptr.at(i));
		}
		left_node->childptr.push_back(splitenode->childptr.at(divide_port));
		for (size_t i = divide_port+1; i < order; ++i)
		{
			right_node->keyItem.push_back(splitenode->keyItem.at(i));
			right_node->childptr.push_back(splitenode->childptr.at(i));
		}
		right_node->childptr.push_back(splitenode->childptr.at(order));
		std::shared_ptr<node>new_left_node(left_node);
		std::shared_ptr<node>new_right_node(right_node);
		if (nullptr == splitenode->next)
		{
			dynamic_cast<internalnode<Key>*>(new_left_node.get())->next = new_right_node;
			dynamic_cast<internalnode<Key>*>(new_left_node.get())->prev = splitenode->prev;
			dynamic_cast<internalnode<Key>*>(new_right_node.get())->next = nullptr;
			dynamic_cast<internalnode<Key>*>(new_right_node.get())->prev = new_left_node;
		}
		else
		{
			dynamic_cast<internalnode<Key>*>(new_left_node.get())->next = new_right_node;
			dynamic_cast<internalnode<Key>*>(new_left_node.get())->prev = splitenode->prev;
			dynamic_cast<internalnode<Key>*>(new_right_node.get())->next =splitenode->next;
			dynamic_cast<internalnode<Key>*>(new_right_node.get())->prev = new_left_node;
		}
		p = new_left_node;
		insert_in_parent(splite_parent_node, divide_key, new_right_node);
	}
}

