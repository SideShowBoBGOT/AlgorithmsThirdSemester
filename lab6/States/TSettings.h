//
// Created by choleraplague on 24.11.22.
//

#ifndef UNTITLED1_TSETTINGS_H
#define UNTITLED1_TSETTINGS_H

#include "../VisualModels/TParent.h"
#include "../Logic/Enums/NNDifficulty.h"

class TSettings : public TParent {
	public:
	TSettings();
	virtual ~TSettings() override=default;
	
	public:
	TControl* TwoPlayers = nullptr;
	TControl* ThreePlayers = nullptr;
	TControl* FourPlayers = nullptr;
	TControl* EasyButton = nullptr;
	TControl* MediumButton = nullptr;
	TControl* HardButton = nullptr;
	TControl* QuitButton = nullptr;
	
	public:
	int GetNumberOfPlayers();
	NDifficulty GetDifficulty();
	
	public:
	TControl* DifficultyLabel = nullptr;
	TControl* PlayersLabel = nullptr;
	
	protected:
	void OnQuitButton(TControl* obj);
	void OnSelectPlayers(TControl* obj);
	void OnSelectDifficulty(TControl* obj);
};


#endif //UNTITLED1_TSETTINGS_H
