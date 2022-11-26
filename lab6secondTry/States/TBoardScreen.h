//
// Created by choleraplague on 25.11.22.
//

#ifndef UNTITLED1_TBOARDSCREEN_H
#define UNTITLED1_TBOARDSCREEN_H

#include <memory>
#include "../VisualModels/TVisualObject.h"

class TBoardScreen : public IVisual {
	public:
	TBoardScreen();
	virtual ~TBoardScreen() override;
	
	public:
	virtual void HandleEvents() override;
	virtual void Clean() override;
	virtual void Render() override;
	
	public:
	std::unique_ptr<TVisualObject> PutButton = nullptr;
	std::unique_ptr<TVisualObject> TakeButton = nullptr;
	std::unique_ptr<TVisualObject> EndTurnButton = nullptr;
	std::unique_ptr<TVisualObject> QuitButton = nullptr;
	
	
};
//101x144

#endif //UNTITLED1_TBOARDSCREEN_H
