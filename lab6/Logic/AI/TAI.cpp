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
	auto pl = ss->CurrentPlayer();
	m_bIsFirstMove = pl->FirstMove();
	InitRoot();
	BuildSuccessorTree();
	if(not m_pRoot->Children.empty()) {
		MaxPowN(m_pRoot);
		auto child = FindMaxNode(m_pRoot);
		auto& unusedCards = ss->UnusedCards();
		auto& playCards = ss->PlayCards();
		auto& localCards = ss->CurrentPlayer()->Cards();
		
		unusedCards = child->UnusedCards;
		playCards = child->PlayCards;
		localCards = child->Previous.LocalCards;
		
//		// Test
//		for(auto c : localCards) {
//			if(std::find(playCards.begin(), playCards.end(), c)!=playCards.end()) {
//				throw "sddsd";
//			}
//		}
	}
	m_pRoot->Current.Player->FirstMove(false);
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

void TAI::Utility(SRoundNode* node) {
	std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,10000000);
    for(auto& [player, value] : node->Values) {
    	value = dist(rng);
    }
}

TAI::SRoundNode*TAI::FindMaxNode(TAI::SRoundNode*node) {
	auto& player = node->Current.Player;
	SRoundNode* mm = nullptr;
	auto i = 0;
	for(auto& child : node->Children) {
		auto cur = FindValueByPlayer(child, player);
		if(i<=cur) {
			mm = child;
		}
	}
	return mm;
}

unsigned long TAI::FindMaxValue(TAI::SRoundNode*node) {
	auto& player = node->Current.Player;
	auto i = 0l;
	for(auto& child : node->Children) {
		auto val = FindValueByPlayer(child, player);
		if(i<val) i = val;
	}
	return i;
}

unsigned long TAI::FindMinValue(TAI::SRoundNode*node, const std::shared_ptr<TPlayer>& player) {
	auto i = unsigned(-1);
	for(auto& child : node->Children) {
		auto val = FindValueByPlayer(child, player);
		if(i>val) i = val;
	}
	return i;
}

unsigned long TAI::FindValueByPlayer(TAI::SRoundNode*node, const std::shared_ptr<TPlayer>& player) {
	auto& vv = node->Values;
	auto it = std::find_if(vv.begin(), vv.end(),
		[player](const auto& el) {
			return el.first==player;
		}
	);
	return it->second;
}

void TAI::BuildSuccessorTree() {
	auto& ss = TLogic::Get()->Session;
	auto depth = s_arrDifficulties[ss->Difficulty()];
	BuildBranches(m_pRoot, depth, 0);
}

void TAI::InitRoot() {
	auto& ss = TLogic::Get()->Session;
	m_pRoot = new SRoundNode();
	UpdateValues(m_pRoot);
	
	auto player = ss->CurrentPlayer();
	m_pRoot->Current.Player = player;
	m_pRoot->Current.LocalCards = player->Cards();
	
	m_pRoot->PlayCards = ss->PlayCards();
	m_pRoot->UnusedCards = ss->UnusedCards();
}

void TAI::BuildBranches(TAI::SRoundNode* parent, const int& maxDepth, int depth) {
	if(maxDepth==depth) return;
	CreateChildren(parent, depth);
	for(auto& c : parent->Children) {
		BuildBranches(c, maxDepth, depth + 1);
	}
}

void TAI::CreateChildren(SRoundNode* parent, const int& depth) {
	auto perms = TPermutator<std::shared_ptr<TCard>>::GetPermutator(parent->Current.LocalCards);
	auto takesFromLocal = TakeCardGroup();
	
	for(auto& p : perms) {
		if(CheckTake(p)) takesFromLocal.emplace_back(p);
//		if(m_bIsFirstMove and p.size()!=3) continue;
//		OnPutMakeNode(parent, std::move(p));
	}
	
	if(not m_bIsFirstMove and not takesFromLocal.empty()) {
		auto pairs = CreateInterchangablePairs(std::move(takesFromLocal), parent);
		for(auto& p : pairs) {
			OnTakeMakeNode(parent, std::move(p));
		}
	}
}

std::vector<TAI::InterchangablePair> TAI::CreateInterchangablePairs(TakeCardGroup&& fromLocal, SRoundNode* node) {
	auto& ss = TLogic::Get()->Session;
	auto pairs = std::vector<TAI::InterchangablePair>();
	auto fromPlay = GetTakesFromPlay(node);
	if(not fromPlay.empty()) {
		for(auto& fl:fromLocal) {
			for(auto& fp:fromPlay) {
				if(ss->CheckTryTake(fl, fp)) {
					pairs.emplace_back(fl, fp);
				}
			}
		}
	}
	return pairs;
}

TAI::TakeCardGroup TAI::GetTakesFromPlay(SRoundNode* node) {
	for(auto& c : node->PlayCards) m_vTakeCards[c->Value()].emplace_back(c);
	auto fromPlay = TakeCardGroup();
	for(auto& v : m_vTakeCards) {
		if(CheckTake(v)) {
			fromPlay.emplace_back(std::move(v));
		}
	}
	ClearTakeCards();
	return fromPlay;
}

void TAI::DeleteTree(SRoundNode* node) {
	for(auto& c : node->Children) {
		DeleteTree(c);
	}
	delete node;
}

void TAI::OnPutMakeNode(TAI::SRoundNode*parent,
	std::vector<std::shared_ptr<TCard>>&& permutation) {
	
	auto child = new SRoundNode();
	child->Parent = parent;
	parent->Children.emplace_back(child);
	UpdateValues(child);
	UpdatePlayers(child);
	OnPutUpdateLocalCards(child, std::move(permutation));
	UpdateUnusedCards(child);
	OnPutUpdatePlayCards(child);
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

void TAI::OnPutUpdateLocalCards(SRoundNode* child,
	std::vector<std::shared_ptr<TCard>>&& permutation) {
	child->Previous.LocalCards = std::move(permutation);
	UpdateLocalCardsCurrent(child);
}

void TAI::UpdateLocalCardsCurrent(SRoundNode* child) {
	auto parent = child->Parent;
	auto childCurPlayer = child->Current.Player;
	while(true) {
		if(not parent) break;
		if(parent->Previous.Player == childCurPlayer) {
			child->Current.LocalCards = parent->Previous.LocalCards;
			return;
		}
		parent = parent->Parent;
	}
	child->Current.LocalCards = child->Current.Player->Cards();
}


void TAI::UpdateUnusedCards(TAI::SRoundNode* node) {
	auto& ss = TLogic::Get()->Session;
	auto parent = node->Parent;
	auto unused = parent->UnusedCards;
	auto& local = node->Previous.LocalCards;
	while(local.size() < ss->PlayCardsNum() and not unused.empty()) {
		local.push_back(unused.back());
		unused.pop_back();
	}
	node->UnusedCards = std::move(unused);
}

void TAI::OnPutUpdatePlayCards(TAI::SRoundNode* node) {
	auto parent = node->Parent;
	auto& pclc = parent->Current.LocalCards;
	auto& cplc = node->Previous.LocalCards;
	node->PlayCards = parent->PlayCards;
	for(auto& c : pclc) {
		if(std::find(cplc.begin(), cplc.end(), c)==cplc.end()) {
			node->PlayCards.push_back(c);
		}
	}
}

void TAI::ClearTakeCards() {
	for(auto& cc : m_vTakeCards) {
		cc.clear();
	}
}

bool TAI::CheckTake(const std::vector<std::shared_ptr<TCard>>& takeCards) {
	auto& ss = TLogic::Get()->Session;
	return ss->CheckTake(takeCards);
}

TAI::SRoundNode*TAI::OnTakeMakeNode(TAI::SRoundNode*parent, TAI::InterchangablePair&& inter) {
	auto child = new SRoundNode();
	child->Parent = parent;
	parent->Children.emplace_back(child);
	UpdateValues(child);
	UpdatePlayers(child);
	auto&& [fromLocal, fromPlay] = inter;
	OnTakeUpdateLocalCards(child, std::move(fromLocal));
	UpdateUnusedCards(child);
	OnTakeUpdatePlayCards(child, std::move(fromPlay));
}

void TAI::OnTakeUpdateLocalCards(TAI::SRoundNode*child, std::vector<std::shared_ptr<TCard>>&& fromLocal) {
	child->Previous.LocalCards = child->Parent->Current.LocalCards;
	auto& pr = child->Previous.LocalCards;
	for(auto& c : fromLocal) {
		if(std::find(pr.begin(), pr.end(), c)!=pr.end()) {
			pr.erase(std::remove(pr.begin(), pr.end(), c), pr.end());
		}
	}
	UpdateLocalCardsCurrent(child);
}

void TAI::OnTakeUpdatePlayCards(TAI::SRoundNode*child, std::vector<std::shared_ptr<TCard>>&& fromPlay) {
	child->PlayCards = child->Parent->PlayCards;
	auto& pl = child->PlayCards;
	for(auto& c : fromPlay) {
		if(std::find(pl.begin(), pl.end(), c)!=pl.end()) {
			pl.erase(std::remove(pl.begin(), pl.end(), c), pl.end());
		}
	}
}







