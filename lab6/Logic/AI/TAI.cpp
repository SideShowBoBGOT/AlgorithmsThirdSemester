//
// Created by choleraplague on 05.12.22.
//

#include <random>
#include "TAI.h"
#include "../../GameSingletons/TLogic.h"
#include "../Notify/SAIEndTurn.h"
#include "../../Other/TPermutator.h"


static constexpr std::array<int, 3> s_arrDifficulties = { 3, 4, 5 };

void TAI::Move() {
	TLogic::Get()->AIThread = std::thread([this](){ DoMove(); });
}

void TAI::DoMove() {
	auto& ss = TLogic::Get()->Session;
	BuildSuccessorTree();
	MaxPowN(m_pRoot);
	DeleteTree(m_pRoot);
	m_pRoot = nullptr;
	ss->SendNotify(std::make_shared<SAIEndTurn>());
}

void TAI::MaxPowN(TAI::SRoundNode* node) {
	auto& children = node->Children;
	if(children.empty()) {
		Utility(node);
		return;
	}
	for(auto& child : children) {
		MaxPowN(child);
	}
	for(auto& [player, value] : node->Values) {
		if(player==node->Current.Player) {
			value = FindMaxValue(node);
			continue;
		}
		value = FindMinValue(node, player);
	}
}

int TAI::FindMaxValue(TAI::SRoundNode*node) {
	auto& player = node->Current.Player;
	auto i = 0;
	for(auto& child : node->Children) {
		i = std::max(i, FindValueByPlayer(child, player));
	}
	return i;
}

int TAI::FindMinValue(TAI::SRoundNode*node, const std::shared_ptr<TPlayer>& player) {
	auto i = 0;
	for(auto& child : node->Children) {
		i = std::min(i, FindValueByPlayer(child, player));
	}
	return i;
}

int TAI::FindValueByPlayer(TAI::SRoundNode*node, const std::shared_ptr<TPlayer>& player) {
	auto& vv = node->Values;
	auto it = std::find_if(vv.begin(), vv.end(),
		[this, player](const auto& el) {
			return el.first==player;
		}
	);
	return it->second;
}

void TAI::BuildSuccessorTree() {
	auto& ss = TLogic::Get()->Session;
	InitRoot();
	auto depth = s_arrDifficulties[ss->Difficulty()];
	BuildBranches(m_pRoot, depth, 0);
}

void TAI::InitRoot() {
	auto& ss = TLogic::Get()->Session;
	m_pRoot = new SRoundNode();
	
	auto player = ss->CurrentPlayer();
	m_pRoot->Current.Player = player;
	m_pRoot->Current.LocalCards = player->Cards();
	
	m_pRoot->PlayCards = ss->PlayCards();
	m_pRoot->UnusedCards = ss->UnusedCards();
}

void TAI::BuildBranches(TAI::SRoundNode* node, const int& maxDepth, int depth) {
	if(maxDepth==depth) return;
	auto perms = TPermutator<std::shared_ptr<TCard>>::GetPermutator(node->Current.LocalCards);
	for(auto& p : perms) {
		auto c = MakeNode(node, std::move(p));
		BuildBranches(c, maxDepth, depth + 1);
	}
}

void TAI::DeleteTree(SRoundNode* node) {
	for(auto& c : node->Children) {
		DeleteTree(c);
	}
	delete node;
}

TAI::SRoundNode* TAI::MakeNode(TAI::SRoundNode*parent,
	std::vector<std::shared_ptr<TCard>>&& permutation) {
	
	auto child = new SRoundNode();
	child->Parent = parent;
	parent->Children.emplace_back(child);
	UpdateValues(child);
	UpdatePlayers(child);
	UpdateLocalCards(child, std::move(permutation));
	UpdateUnusedCards(child);
	UpdatePlayCards(child);
	return child;
}

void TAI::UpdateValues(SRoundNode* child) {
	auto& ss = TLogic::Get()->Session;
	for(const auto& p : ss->Players()) {
		child->Values.emplace_back(p, 0);
	}
}

void TAI::UpdatePlayers(TAI::SRoundNode*child) {
	auto& ss = TLogic::Get()->Session;
	auto parent = child->Parent;
	child->Previous.Player = parent->Current.Player;
	child->Current.Player = ss->NextPlayer(parent->Current.Player);
}

void TAI::UpdateLocalCards(SRoundNode* child,
	std::vector<std::shared_ptr<TCard>>&& permutation) {
	
	child->Previous.LocalCards = std::move(permutation);
	// Get child->Current.LocalCards up the tree structure
	auto childCurPlayer = child->Current.Player;
	auto wasInTreeBefore = false;
	auto node = child->Parent;
	while(true) {
		if(not node) break;
		if(node->Previous.Player == childCurPlayer) {
			wasInTreeBefore = true;
			break;
		}
		node = node->Parent;
	}
	if(wasInTreeBefore) {
		child->Current.LocalCards = node->Previous.Player->Cards();
		return;
	}
	child->Current.LocalCards = child->Current.Player->Cards();
}

void TAI::UpdateUnusedCards(TAI::SRoundNode* child) {
	auto& ss = TLogic::Get()->Session;
	auto parent = child->Parent;
	auto unused = parent->UnusedCards;
	auto& local = child->Previous.LocalCards;
	while(local.size() < ss->PlayCardsNum()) {
		local.emplace_back(unused.back());
		unused.pop_back();
	}
	child->UnusedCards = std::move(unused);
}

void TAI::UpdatePlayCards(TAI::SRoundNode*child) {
	auto parent = child->Parent;
	auto& pclc = parent->Current.LocalCards;
	auto& cplc = child->Previous.LocalCards;
	child->PlayCards = parent->PlayCards;
	std::set_difference(pclc.begin(), pclc.end(),
		cplc.begin(), cplc.end(), std::back_inserter(child->PlayCards));
}



int TAI::Utility(SRoundNode* node) {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,100);
	return dist(rng);
}






