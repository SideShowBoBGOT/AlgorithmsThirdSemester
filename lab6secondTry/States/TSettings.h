//
// Created by choleraplague on 24.11.22.
//

#ifndef UNTITLED1_TSETTINGS_H
#define UNTITLED1_TSETTINGS_H

#include "../Models/TGameState.h"
#include "../Models/TVisualObject.h"

class TSettings : public IGameState {
	public:
	TSettings();
	virtual ~TSettings() override;
	
	public:
	virtual void HandleEvents() override;
	virtual void Clean() override;
	virtual void Render() override;
	
	public:
	TVisualObject* TwoPlayers = nullptr;
	TVisualObject* ThreePlayers = nullptr;
	TVisualObject* FourPlayers = nullptr;
	TVisualObject* QuitButton = nullptr;
	
	public:
	void OnQuitButton(TVisualObject* obj);
	void OnSelectPlayers(TVisualObject* obj);
};


#endif //UNTITLED1_TSETTINGS_H
