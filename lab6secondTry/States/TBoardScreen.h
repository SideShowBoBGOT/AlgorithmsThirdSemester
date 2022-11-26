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
	std::shared_ptr<TVisualObject> PutButton 		= nullptr;
	std::shared_ptr<TVisualObject> TakeButton 		= nullptr;
	std::shared_ptr<TVisualObject> EndTurnButton 	= nullptr;
	std::shared_ptr<TVisualObject> QuitButton 		= nullptr;
	std::shared_ptr<TVisualObject> NextPageButton 	= nullptr;
	std::shared_ptr<TVisualObject> PrevPageButton 	= nullptr;
	std::shared_ptr<TVisualObject> DeselectButton 	= nullptr;
	
	public:
	std::shared_ptr<TVisualObject> AIOneLabel 		= nullptr;
	std::shared_ptr<TVisualObject> AITwoLabel 		= nullptr;
	std::shared_ptr<TVisualObject> AIThreeLabel 	= nullptr;
	
	protected:
	static constexpr int CardSlotSize = 8;
	std::array<std::shared_ptr<TVisualObject>, CardSlotSize> CardSlots;
	
	protected:
	void OnPutButton(std::shared_ptr<TVisualObject> obj);
	void OnTakeButton(std::shared_ptr<TVisualObject> obj);
	void OnEndTurnButton(std::shared_ptr<TVisualObject> obj);
	void OnQuitButton(std::shared_ptr<TVisualObject> obj);
	void OnNextPageButton(std::shared_ptr<TVisualObject> obj);
	void OnPrevPageButton(std::shared_ptr<TVisualObject> obj);
	void OnDeselectButton(std::shared_ptr<TVisualObject> obj);
};
//101x144

#endif //UNTITLED1_TBOARDSCREEN_H
