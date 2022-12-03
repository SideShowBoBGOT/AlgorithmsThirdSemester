//
// Created by choleraplague on 25.11.22.
//

#ifndef UNTITLED1_TBOARDSCREEN_H
#define UNTITLED1_TBOARDSCREEN_H

#include <array>
#include "../VisualModels/TAIBlock.h"
#include "../VisualModels/TVisualCard.h"
#include "../Logic/TCard.h"

class TBoardScreen : public TParent {
	public:
	TBoardScreen();
	virtual ~TBoardScreen() override=default;
	
	public:
	TAutoAlignArea* ButtonPanel = nullptr;
	TControl* PutButton 		= nullptr;
	TControl* TakeButton 		= nullptr;
	TControl* EndTurnButton 	= nullptr;
	TControl* QuitButton 		= nullptr;
	TControl* DeselectButton 	= nullptr;
	
	public:
	TAutoAlignArea* AIPanel 	= nullptr;
	TAIBlock* AIOneLabel 		= nullptr;
	TAIBlock* AITwoLabel 		= nullptr;
	TAIBlock* AIThreeLabel 		= nullptr;
	
	public:
	TAutoAlignArea* LocalCards 	= nullptr;
	TAutoAlignArea* PlayCards 	= nullptr;
	
	public:
	void StartGame();
	
	protected:
	void OnPutButton(TControl* obj);
	void OnTakeButton(TControl* obj);
	void OnEndTurnButton(TControl* obj);
	void OnQuitButton(TControl* obj);
	void OnDeselectButton(TControl* obj);
	
	protected:
	void UpdateVisuals();
	void UpdateCards(TAutoAlignArea* area, const std::vector<std::shared_ptr<TCard>>& cards);
	void SetVisibleAILabels();
	void SetVisualCards();
	void LockInterface();
	void UnLockInterface();
	
	protected:
	std::vector<std::shared_ptr<TCard>> FromVisToLogCards(const std::vector<TControl*>& vis);
	std::vector<TControl*> FilterSelectedCards(const std::vector<TControl*>& vis);
	
	protected:
	std::vector<std::pair<TVisualCard*, std::shared_ptr<TCard>>> m_vVisLogPairs;
};
//101x144

#endif //UNTITLED1_TBOARDSCREEN_H
