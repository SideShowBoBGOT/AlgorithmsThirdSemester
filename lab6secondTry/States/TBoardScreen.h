//
// Created by choleraplague on 25.11.22.
//

#ifndef UNTITLED1_TBOARDSCREEN_H
#define UNTITLED1_TBOARDSCREEN_H

#include <array>
#include "../VisualModels/TVisualParent.h"

class TBoardScreen : public TVisualParent {
	public:
	TBoardScreen();
	virtual ~TBoardScreen() override=default;
	
	public:
	TVisualObject* PutButton 		= nullptr;
	TVisualObject* TakeButton 		= nullptr;
	TVisualObject* EndTurnButton 	= nullptr;
	TVisualObject* QuitButton 		= nullptr;
	TVisualObject* NextPageButton 	= nullptr;
	TVisualObject* PrevPageButton 	= nullptr;
	TVisualObject* DeselectButton 	= nullptr;
	
	public:
	TVisualObject* AIOneLabel 		= nullptr;
	TVisualObject* AITwoLabel 		= nullptr;
	TVisualObject* AIThreeLabel 	= nullptr;
	
	protected:
	static constexpr int CardSlotSize = 8;
	std::array<TVisualObject*, CardSlotSize> CardSlots;
	
	protected:
	void OnPutButton(TVisualObject* obj);
	void OnTakeButton(TVisualObject* obj);
	void OnEndTurnButton(TVisualObject* obj);
	void OnQuitButton(TVisualObject* obj);
	void OnNextPageButton(TVisualObject* obj);
	void OnPrevPageButton(TVisualObject* obj);
	void OnDeselectButton(TVisualObject* obj);
	
	protected:
	void LockInterface();
	void UnLockInterface();
};
//101x144

#endif //UNTITLED1_TBOARDSCREEN_H
