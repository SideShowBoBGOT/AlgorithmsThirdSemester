//
// Created by choleraplague on 25.11.22.
//

#ifndef UNTITLED1_TBOARDSCREEN_H
#define UNTITLED1_TBOARDSCREEN_H

#include <array>
#include "../VisualModels/TAutoAlignArea.h"

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
	TControl* NextPageButton 	= nullptr;
	TControl* PrevPageButton 	= nullptr;
	TControl* DeselectButton 	= nullptr;
	
	public:
	TAutoAlignArea* AIPanel 	= nullptr;
	TControl* AIOneLabel 		= nullptr;
	TControl* AITwoLabel 		= nullptr;
	TControl* AIThreeLabel 		= nullptr;
	
	protected:
	static constexpr int CardSlotSize = 8;
	std::array<TControl*, CardSlotSize> CardSlots;
	
	protected:
	void OnPutButton(TControl* obj);
	void OnTakeButton(TControl* obj);
	void OnEndTurnButton(TControl* obj);
	void OnQuitButton(TControl* obj);
	void OnNextPageButton(TControl* obj);
	void OnPrevPageButton(TControl* obj);
	void OnDeselectButton(TControl* obj);
	
	protected:
	void LockInterface();
	void UnLockInterface();
};
//101x144

#endif //UNTITLED1_TBOARDSCREEN_H
