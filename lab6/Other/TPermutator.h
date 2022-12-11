//
// Created by choleraplague on 11.12.22.
//

#ifndef UNTITLED1_TPERMUTATOR_H
#define UNTITLED1_TPERMUTATOR_H

#include <vector>

template<typename T>
class TPermutator {
	public:
	TPermutator()=delete;
	~TPermutator()=delete;
	
	public:
	static std::vector<std::vector<T>> GetPermutator(const std::vector<T>& v);
	
	private:
	struct SNode {
		size_t Index = -1;
		std::vector<SNode*> Children;
	};
	
	private:
	static void BuildTree(SNode* root, const std::vector<T>& v);
	static void DoBuildTree(SNode* node, const std::vector<T>& v);
	static void CrawlTree(SNode* node, std::vector<T>& cur,
		std::vector<std::vector<T>>& perms, const std::vector<T>& v);
	static void DeleteTree(SNode* node);
};

template<typename T>
std::vector<std::vector<T>> TPermutator<T>::GetPermutator(const std::vector<T>& v) {
	auto root = new SNode();
	root->Index = -1;
	BuildTree(root, v);
	auto cur = std::vector<T>();
	auto perms = std::vector<std::vector<T>> ();
	CrawlTree(root, cur, perms, v);
	DeleteTree(root);
	return perms;
}

template<typename T>
void TPermutator<T>::BuildTree(SNode* root, const std::vector<T>& v) {
	for(auto i=0;i<v.size();++i) {
		auto c = new SNode();
		c->Index = i;
		DoBuildTree(c, v);
		root->Children.emplace_back(c);
	}
}

template<typename T>
void TPermutator<T>::DoBuildTree(SNode* node, const std::vector<T>& v) {
	auto index = node->Index + 1;
	while(index<v.size()) {
		auto child = new SNode();
		child->Index = index;
		DoBuildTree(child, v);
		node->Children.emplace_back(child);
		++index;
	}
}

template<typename T>
void TPermutator<T>::CrawlTree(SNode* node, std::vector<T>& cur,
	std::vector<std::vector<T>>& perms, const std::vector<T>& v) {
	
	for(SNode* c : node->Children) {
		auto vv = cur;
		vv.emplace_back(v[c->Index]);
		CrawlTree(c, vv, perms, v);
	}
	if(node->Index!=-1) perms.emplace_back(cur);
}

template<typename T>
void TPermutator<T>::DeleteTree(SNode* node) {
	for(SNode* c : node->Children) DeleteTree(c);
	delete node;
}

#endif //UNTITLED1_TPERMUTATOR_H
