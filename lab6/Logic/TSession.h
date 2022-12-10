//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TSESSION_H
#define UNTITLED1_TSESSION_H

#include <mutex>

#include "TPlayer.h"
#include "Enums/NNDifficulty.h"
#include "Notify/INotify.h"
#include "AI/TAI.h"
#include "../Other/TThreadSafeQueue.h"

class TSession {
	public:
	TSession()=delete;
	TSession(NDifficulty diff, int playersNumber);
	virtual ~TSession()=default;
	
	#define DECL_VECTOR(xx, type) \
		protected:            \
		type m_v##xx;      \
        public:                        \
		virtual type& xx();
	
		DECL_VECTOR(Players, std::vector<std::shared_ptr<TPlayer>>);
		DECL_VECTOR(PlayCards, std::vector<std::shared_ptr<TCard>>);
		DECL_VECTOR(SparseCards, std::vector<std::shared_ptr<TCard>>);
		DECL_VECTOR(UnusedCards, std::vector<std::shared_ptr<TCard>>);
		DECL_VECTOR(Cards, std::vector<std::shared_ptr<TCard>>);
	#undef DECL_VECTOR
	
	#define DECL(xx, type, prefix, value) \
		protected:            \
		type m_##prefix##xx = value;      \
        public:                        \
		virtual type xx();
	
		DECL(Difficulty, NDifficulty, x, NDifficulty::Medium);
		DECL(PlayersNumber, int, i, 2);
		DECL(LocalPlayer, std::shared_ptr<TPlayer>, p, nullptr);
		DECL(CurrentPlayer, std::shared_ptr<TPlayer>, p, nullptr);
		DECL(Trump, NCardType, x, NCardType::Hearts);
	#undef DECL
	
	protected:
	TThreadSafeQueue<std::shared_ptr<INotify>> m_stNotifies;
	public:                        \
	virtual TThreadSafeQueue<std::shared_ptr<INotify>>& Notifies();
	
	public:
	void NextTurn();
	bool TryTake(const std::vector<std::shared_ptr<TCard>>& selectedOwnCards, const std::vector<std::shared_ptr<TCard>>& selectedPlayCards);
	bool TryPut(const std::vector<std::shared_ptr<TCard>>& selectedOwnCards);
	void SendNotify(const std::shared_ptr<INotify>& n);

	protected:
	void DistributeCardsAmongPlayers();
	void CreateCards();
	void TossCards();
	void GiveCards();
	void CalculateAI();
	
	static void VectorCardDifference(std::vector<std::shared_ptr<TCard>>& vOne, const std::vector<std::shared_ptr<TCard>>& vTwo);
	static bool CheckSelected(const std::vector<std::shared_ptr<TCard>>& selectedCards);
	bool IsAnyPlayerHasCard(const std::shared_ptr<TCard>& c);
	void RandomPlayer();
	
	protected:
	std::shared_ptr<TAI> m_pAI = nullptr;
	std::mutex m_xNotifiesMutex;
};


#endif //UNTITLED1_TSESSION_H
