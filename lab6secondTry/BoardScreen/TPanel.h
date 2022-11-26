//
// Created by choleraplague on 25.11.22.
//

#ifndef UNTITLED1_TPANEL_H
#define UNTITLED1_TPANEL_H

#include "TCardsSelector.h"
#include "TAIPlayersPanel.h"

class TPanel : TVisualObject {
	public:
	TPanel();
	virtual ~TPanel() override;
	
	public:
	virtual void HandleEvents() override;
	virtual void Draw() override;
	virtual void Clean() override;
	
	public:
	TVisualObject* PutButton = nullptr;
	TVisualObject* TakeButton = nullptr;
	TVisualObject* EndTurnButton = nullptr;
	TVisualObject* QuitButton = nullptr;
	
	public:
	TAIPlayersPanel* PlayersPanel = nullptr;
	TCardsSelector* CardsSelector = nullptr;
	
	protected:
	void OnQuitButton(TVisualObject* obj);
};


#endif //UNTITLED1_TPANEL_H
