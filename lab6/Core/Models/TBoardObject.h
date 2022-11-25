//
// Created by choleraplague on 14.11.22.
//

#ifndef LAB6_TBOARDOBJECT_H
#define LAB6_TBOARDOBJECT_H


#include "TPlayerObject.h"

namespace Logic {
	
	class TBoardObject {
		public:
		TBoardObject()=default;
		virtual ~TBoardObject()=default;
		
		
		public:
		virtual std::shared_ptr<TPlayerObject>& CurrentPlayer();
		virtual std::shared_ptr<TPlayerObject>& LocalPlayer();
		
		public:
		virtual std::vector<std::shared_ptr<TCardObject>>&  PlayCards();
		virtual std::vector<std::shared_ptr<TCardObject>>&  SparseCards();
		

		
		public:
		virtual void OnGameBegin();
		virtual void NextTurn();
		
		protected:
		virtual void BeginTurnFor(const std::shared_ptr<TPlayerObject>& player);
		virtual void EndTurnFor(const std::shared_ptr<TPlayerObject>& player);
		
		protected:
		virtual std::shared_ptr<TPlayerObject> PickPlayerToGiveCards();
		virtual std::shared_ptr<TPlayerObject> PickNextPlayerToPlay();
		virtual void GivePlayerSparseCards(const std::shared_ptr<TPlayerObject>& player);
		virtual std::shared_ptr<TPlayerObject> PickPlayerToPlayFirst();
		
		protected:
		std::shared_ptr<TPlayerObject> m_pCurrentPlayer = nullptr;
		std::shared_ptr<TPlayerObject> m_pLocalPlayer = nullptr;
		std::vector<std::shared_ptr<TPlayerObject>> m_vPlayers;
		
		protected:
		NCardGroup m_xCozyr = NCardGroup::None;
		std::vector<std::shared_ptr<TCardObject>> m_vPlayCards;
		std::vector<std::shared_ptr<TCardObject>> m_vSparseCards;
		std::vector<std::shared_ptr<TCardObject>> m_vAllCards;
	};
	
} // Logic

#endif //LAB6_TBOARDOBJECT_H
