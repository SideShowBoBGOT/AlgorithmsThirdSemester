//
// Created by choleraplague on 24.11.22.
//

#ifndef UNTITLED1_TSETTINGS_H
#define UNTITLED1_TSETTINGS_H

#include "../VisualModels/TVisualParent.h"
#include "../Logic/NNDifficulty.h"

class TSettings : public TVisualParent {
	public:
	TSettings();
	virtual ~TSettings() override=default;
	
	public:
	TVisualObject* TwoPlayers = nullptr;
	TVisualObject* ThreePlayers = nullptr;
	TVisualObject* FourPlayers = nullptr;
	TVisualObject* EasyButton = nullptr;
	TVisualObject* MediumButton = nullptr;
	TVisualObject* HardButton = nullptr;
	TVisualObject* QuitButton = nullptr;
	
	public:
	int GetNumberOfPlayers();
	NDifficulty GetDifficulty();
	
	public:
	TVisualObject* DifficultyLabel = nullptr;
	TVisualObject* PlayersLabel = nullptr;
	
	protected:
	void OnQuitButton(TVisualObject* obj);
	void OnSelectPlayers(TVisualObject* obj);
	void OnSelectDifficulty(TVisualObject* obj);
};


#endif //UNTITLED1_TSETTINGS_H
