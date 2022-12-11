//
// Created by choleraplague on 05.12.22.
//

#include <chrono>
#include "TAI.h"
#include "../../GameSingletons/TLogic.h"
#include "../Notify/SAIEndTurn.h"
#include "../../Other/TPermutator.h"

void TAI::Move() {
	TLogic::Get()->AIThread = std::thread([this](){ DoMove(); });
}

void TAI::DoMove() {
	auto& ss = TLogic::Get()->Session;
	BuildSuccessorTree();
	
	
	DeleteTree(m_pRoot);
	m_pRoot = nullptr;
	ss->SendNotify(std::make_shared<SAIEndTurn>());
}

TAI::SNode*TAI::MaxPowN(TAI::SNode*node, bool isMax) {

}

void TAI::BuildSuccessorTree() {
	auto& ss = TLogic::Get()->Session;
	InitRoot();
	BuildBranches(m_pRoot, ss->Difficulty(), 0);
}

void TAI::InitRoot() {
	auto& ss = TLogic::Get()->Session;
	m_pRoot = new SNode();
	m_pRoot->Parent = nullptr;
	m_pRoot->PlayCards = ss->PlayCards();
	m_pRoot->UnusedCards = ss->UnusedCards();
	m_pRoot->LocalCards = ss->CurrentPlayer()->Cards();
}

void TAI::BuildBranches(TAI::SNode* node, const int& maxDepth, int depth) {
	if(maxDepth==depth) return;
	auto perms = TPermutator<std::shared_ptr<TCard>>::GetPermutator(node->LocalCards);
	for(auto& p : perms) {
		auto c = MakeNode(node, std::move(p));
		BuildBranches(c, maxDepth, depth + 1);
	}
}

TAI::SNode* TAI::MakeNode(TAI::SNode*parent,
	std::vector<std::shared_ptr<TCard>>&& perm) {
	
	auto c = new SNode();
	c->Parent = parent;
	parent->Children.emplace_back(c);
	UpdateLocalCards(c, std::move(perm));
	UpdateUnusedCards(c);
	UpdatePlayCards(c);
	
	return c;
}

void TAI::UpdateLocalCards(TAI::SNode* node,
	std::vector<std::shared_ptr<TCard>>&& perm) {
	node->LocalCards = std::move(perm);
}

void TAI::UpdateUnusedCards(TAI::SNode* child) {
	auto& ss = TLogic::Get()->Session;
	auto parent = child->Parent;
	auto unused = parent->UnusedCards;
	auto& local = child->LocalCards;
	while(local.size() < ss->PlayCardsNum()) {
		local.emplace_back(unused.back());
		unused.pop_back();
	}
	child->UnusedCards = std::move(unused);
}

void TAI::UpdatePlayCards(TAI::SNode*child) {
	auto parent = child->Parent;
	auto& plc = parent->LocalCards;
	auto & clc = child->LocalCards;
	child->PlayCards = parent->PlayCards;
	std::set_difference(plc.begin(), plc.end(),
		clc.begin(), clc.end(), std::back_inserter(child->PlayCards));
}

void TAI::DeleteTree(SNode* node) {
	for(auto& c : node->Children) {
		DeleteTree(c);
	}
	delete node;
}

int TAI::Utility() {
	return 0;
}