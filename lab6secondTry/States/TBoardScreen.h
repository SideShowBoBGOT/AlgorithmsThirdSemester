//
// Created by choleraplague on 25.11.22.
//

#ifndef UNTITLED1_TBOARDSCREEN_H
#define UNTITLED1_TBOARDSCREEN_H

#include "../Models/TGameState.h"
#include "../BoardScreen/TPanel.h"

class TBoardScreen : public IGameState {
	public:
	TBoardScreen();
	virtual ~TBoardScreen() override;
	
	public:
	virtual void HandleEvents() override;
	virtual void Clean() override;
	virtual void Render() override;
	
	public:
	TPanel* UserPanel = nullptr;
	
};
//101x144

#endif //UNTITLED1_TBOARDSCREEN_H
