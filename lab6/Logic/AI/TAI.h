//
// Created by choleraplague on 05.12.22.
//

#ifndef UNTITLED1_TAI_H
#define UNTITLED1_TAI_H

#include <vector>
#include <memory>
#include <functional>
#include "../Enums/NNCardValue.h"
#include "../../Patterns/TSingleton.h"

class TPlayer;
class TCard;
class TSession;

class TAI : public TSingleton<TAI> {
	friend class TSession;
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
	
	protected:
	struct SRoundNode {
		SRoundNode* Parent = nullptr;
		SPlayerHalf Previous;
		SPlayerHalf Current;
		std::vector<std::shared_ptr<TCard>> PlayCards;
		std::vector<std::shared_ptr<TCard>> UnusedCards;
		std::vector<SRoundNode*> Children;
		std::vector<std::pair<std::shared_ptr<TPlayer>, unsigned long>> Values;
	};
	
	protected:
	void MaxPowN(SRoundNode* node);
	void Utility(SRoundNode* node);
	
	protected:
	void BuildSuccessorTree();
	void InitRoot();
	void BuildBranches(SRoundNode* parent, const int& maxDepth, int depth);
	void DeleteTree(SRoundNode* node);
	
	protected:
	unsigned long FindMaxValue(SRoundNode* node);
	unsigned long FindMinValue(SRoundNode* node, const std::shared_ptr<TPlayer>& player);
	unsigned long FindValueByPlayer(SRoundNode* node, const std::shared_ptr<TPlayer>& player);
	SRoundNode* FindMaxNode(SRoundNode* node);
	
	protected:
	void UpdatePlayers(SRoundNode* child);
	void UpdateValues(SRoundNode* node);
	void UpdateUnusedCards(SRoundNode* node);
	void UpdateLocalCardsCurrent(SRoundNode* node);
	void CreateChildren(SRoundNode* node, const int& depth);
	// PUT
	protected:
	void OnPutMakeNode(SRoundNode* parent,
		std::vector<std::shared_ptr<TCard>>&& permutation);
	void OnPutUpdateLocalCards(SRoundNode* child,
		std::vector<std::shared_ptr<TCard>>&& permutation);
	void OnPutUpdatePlayCards(SRoundNode* node);
	// TAKE
	protected:
	using InterchangablePair = std::pair<std::vector<std::shared_ptr<TCard>>, std::vector<std::shared_ptr<TCard>>>;
	using TakeCardGroup = std::vector<std::vector<std::shared_ptr<TCard>>>;
	std::vector<InterchangablePair> CreateInterchangablePairs(TakeCardGroup&& fromLocal, SRoundNode* node);
	TakeCardGroup GetTakesFromPlay(SRoundNode* node);
	SRoundNode* OnTakeMakeNode(SRoundNode* parent, InterchangablePair&& inter);
	void OnTakeUpdateLocalCards(SRoundNode* child, std::vector<std::shared_ptr<TCard>>&& fromLocal);
	void OnTakeUpdatePlayCards(SRoundNode* child, std::vector<std::shared_ptr<TCard>>&& fromPlay);
	
	protected:
	bool CheckTake(const std::vector<std::shared_ptr<TCard>>& takeCards);
	void ClearTakeCards();
	
	protected:
	SRoundNode* m_pRoot = nullptr;
	bool m_bIsFirstMove = true;
	std::array<std::vector<std::shared_ptr<TCard>>, NCardValue::Size> m_vTakeCards;
};


#endif //UNTITLED1_TAI_H
