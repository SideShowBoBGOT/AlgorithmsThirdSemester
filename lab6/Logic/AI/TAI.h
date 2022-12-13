//
// Created by choleraplague on 05.12.22.
//

#ifndef UNTITLED1_TAI_H
#define UNTITLED1_TAI_H

#include <vector>
#include <memory>
#include <functional>
#include "../../Patterns/TSingleton.h"

class TPlayer;
class TCard;
class TSession;

class TAI : public TSingleton<TAI> {
	public:
	TAI()=default;
	virtual ~TAI()=default;
	
	public:
	void Move();
	std::shared_ptr<TSession> Session = nullptr;
	
	protected:
	void DoMove();
	
	protected:
	struct SPlayerHalf {
		std::shared_ptr<TPlayer> Player;
		std::vector<std::shared_ptr<TCard>> LocalCards;
	};
	
	struct SRoundNode {
		SRoundNode* Parent = nullptr;
		SPlayerHalf Previous;
		SPlayerHalf Current;
		std::vector<std::shared_ptr<TCard>> PlayCards;
		std::vector<std::shared_ptr<TCard>> UnusedCards;
		std::vector<SRoundNode*> Children;
		std::vector<std::pair<std::shared_ptr<TPlayer>, int>> Values;
	};
	
	protected:
	void MaxPowN(SRoundNode* node);
	int Utility(SRoundNode* node);
	
	
	protected:
	void BuildSuccessorTree();
	void InitRoot();
	void BuildBranches(SRoundNode* node, const int& maxDepth, int depth);
	void DeleteTree(SRoundNode* node);
	
	protected:
	int FindMaxValue(SRoundNode* node);
	int FindMinValue(SRoundNode* node, const std::shared_ptr<TPlayer>& player);
	int FindValueByPlayer(SRoundNode* node, const std::shared_ptr<TPlayer>& player);
	
	protected:
	SRoundNode* MakeNode(SRoundNode* parent,
		std::vector<std::shared_ptr<TCard>>&& permutation);
	void UpdatePlayers(SRoundNode* child);
	void UpdateLocalCards(SRoundNode* child,
		std::vector<std::shared_ptr<TCard>>&& permutation);
	void UpdateUnusedCards(SRoundNode* child);
	void UpdatePlayCards(SRoundNode* child);
	void UpdateValues(SRoundNode* child);
	
	protected:
	SRoundNode* m_pRoot = nullptr;
};


#endif //UNTITLED1_TAI_H
