//
// Created by choleraplague on 16.12.22.
//

#include "TTakeTest.h"
#include "../../GameSingletons/TLogic.h"
#include "../TSession.h"

std::vector<std::shared_ptr<TCard>> TTakeTest::GetSetOfCards(NCardValue value) {
	auto& ss = TLogic::Get()->Session;
	auto& cards = ss->Cards();
	auto out = std::vector<std::shared_ptr<TCard>>();
	for(auto i=0;i<NCardType::Size;++i) {
		auto it = std::find_if(cards.begin(), cards.end(),
			[value, i](const auto& c) {
				return c->Value()==value and c->Type()==NCardType(i);
			}
		);
		if(it!=cards.end()) out.emplace_back(*it);
	}
	return out;
}


bool TTakeTest::Test() {
	
	{
		TLogic::Get()->Session = std::make_shared<TSession>(NDifficulty::Medium, 2);
		auto& ss = TLogic::Get()->Session;
		
		auto tens = GetSetOfCards(NCardValue::Ten);
		
		auto sixs = GetSetOfCards(NCardValue::Six);
		//auto sevens = GetSetOfCards(NCardValue::Valet);
		// auto queens = GetSetOfCards(NCardValue::Queen);
		
		auto local = std::vector<std::shared_ptr<TCard>>();
		std::copy(tens.begin(), tens.end(), std::back_inserter(local));
		
		auto play = std::vector<std::shared_ptr<TCard>>();
		std::copy(sixs.begin(), sixs.end(), std::back_inserter(play));
//		std::copy(sevens.begin(), sevens.end(), std::back_inserter(play));
//		std::copy(queens.begin(), queens.end(), std::back_inserter(play));
		
		auto& ai = ss->Players()[1];
		ai->Cards() = local;
		ai->FirstMove(false);
		ss->PlayCards() = play;
		ss->UnusedCards().clear();
		
		ss->NextTurn();
		TLogic::Get()->AIThread.join();
		
		auto& aiLocal = ai->Cards();
		auto& pl = ss->PlayCards();
		for(auto& ten : tens) {
			if(std::find(aiLocal.begin(), aiLocal.end(), ten)!=aiLocal.end()) {
				throw "Local Take Failed";
			}
		}
		for(auto& six : sixs) {
			if(std::find(pl.begin(), pl.end(), six)!=pl.end()) {
				throw "Play Take Failed";
			}
		}
	}
}
