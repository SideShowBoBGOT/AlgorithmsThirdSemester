//
// Created by choleraplague on 05.12.22.
//

#ifndef UNTITLED1_TAI_H
#define UNTITLED1_TAI_H

#include <vector>
#include <memory>
#include "../../Patterns/TSingleton.h"

class TCard;

class TAI : public TSingleton<TAI> {
	public:
	TAI()=default;
	virtual ~TAI()=default;
	
	public:
	void Move();
	
	protected:
	void DoMove();
	
	protected:
	struct SNode {
		public:
		SNode()=default;
		virtual ~SNode()=default;
		
		public:
		SNode* Parent = nullptr;
		std::vector<std::shared_ptr<TCard>> LocalCards;
		std::vector<std::shared_ptr<TCard>> PlayCards;
		std::vector<std::shared_ptr<TCard>> UnusedCards;
		std::vector<SNode*> Children;
		int Value = 0;
		bool isMax = true;
	};
	
	protected:
	SNode* MaxPowN(SNode* node, bool isMax);
	int Utility();
	
	protected:
	void BuildSuccessorTree();
	void InitRoot();
	void BuildBranches(SNode* node, const int& maxDepth, int depth);
	void DeleteTree(SNode* node);
	
	protected:
	SNode* MakeNode(SNode* parent,
		std::vector<std::shared_ptr<TCard>>&& perm);
	void UpdateLocalCards(SNode* node,
		std::vector<std::shared_ptr<TCard>>&& perm);
	void UpdateUnusedCards(SNode* child);
	void UpdatePlayCards(SNode* child);
	
	protected:
	SNode* m_pRoot = nullptr;

};


#endif //UNTITLED1_TAI_H
