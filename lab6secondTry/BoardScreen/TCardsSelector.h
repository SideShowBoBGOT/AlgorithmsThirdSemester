//
// Created by choleraplague on 25.11.22.
//

#ifndef UNTITLED1_TCARDSSELECTOR_H
#define UNTITLED1_TCARDSSELECTOR_H

#include "TVisualCard.h"
#include <vector>

class TCardsSelector : TVisualObject {
	public:
	TCardsSelector();
	virtual ~TCardsSelector() override;
	
	public:
	virtual void HandleEvents() override;
	virtual void Render() override;
	virtual void Clean() override;
	
	public:
	TVisualObject* NextPage = nullptr;
	TVisualObject* PrevPage = nullptr;
	TVisualObject* Deselect = nullptr;
	
	protected:
	void OnSelectCard(TVisualObject* obj);
	
	protected:
	static constexpr int s_iCardSlots = 7;
	
	public:
	std::vector<TVisualCard*> Cards;
	std::vector<TVisualCard*> SelectedCards;
	
};


#endif //UNTITLED1_TCARDSSELECTOR_H
