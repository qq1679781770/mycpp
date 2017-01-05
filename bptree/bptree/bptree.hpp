#pragma once
#ifndef BPTREE_HPP
#define BPTREE_HPP
#include<vector>
#include<algorithm>
#include<iostream>
#include<memory>
/*the base of all nodes*/
struct node
{
	virtual ~node() {}
};
template<typename Value>
/*the node recording the value*/
struct recordnode:node
{
	Value value;
	explicit recordnode(Value _value) : value(_value) {}
};
template<typename Key>
/*the root of bplustree*/
struct rootnode:node
{
	std::vector<Key>keyItem;
	std::vector<std::shared_ptr<node>>childptr;
	//std::shared_ptr<node>parent;
	//rootnode():{}
};
template<typename Key>
/*the internal of the tree*/
struct internalnode:node
{
	std::vector<Key>keyItem;
	std::vector<std::shared_ptr<node>>childptr;
	std::shared_ptr<node>prev;
	std::shared_ptr<node>next;
	std::shared_ptr<node>parent;
};
template<typename Key>
/*leaf node*/
struct leafnode :node
{
	std::vector<Key>keyItem;
	std::vector<std::shared_ptr<node>>childptr;
	std::shared_ptr<node>prev;
	std::shared_ptr<node>next;
	std::shared_ptr<node>parent;
};
template<typename Key,typename Value>
class bplus_tree
{
private:
	size_t order;//the order of the bplustree
	std::shared_ptr<rootnode<Key>>root;
	std::shared_ptr<node> search_node(std::shared_ptr<node> const& p, Key const& key, bool remove_or_insert);
	void insert_in_parent(std::shared_ptr<node>& p, Key const& key,std::shared_ptr<node>& child_node);
	void splite_node(std::shared_ptr<node> &p);
	void solveunderflow(std::shared_ptr<node>& p);
	void borrow_node(std::shared_ptr<node>& p);
	void combine_node(std::shared_ptr<node>& p);
public:
	bplus_tree(size_t const _order):order(_order), root(nullptr) {}
	bool insert(const Key& key, const Value& value);
	bool remove(const Key& key);
	Value search_value(Key const& key);
	void visit(std::shared_ptr<node>const &_node);
	void print() { visit(root); }
};
#endif  
