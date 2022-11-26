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
	std::shared_ptr<TVisualObject> TwoPlayers = nullptr;
	std::shared_ptr<TVisualObject> ThreePlayers = nullptr;
	std::shared_ptr<TVisualObject> FourPlayers = nullptr;
	std::shared_ptr<TVisualObject> EasyButton = nullptr;
	std::shared_ptr<TVisualObject> MediumButton = nullptr;
	std::shared_ptr<TVisualObject> HardButton = nullptr;
	std::shared_ptr<TVisualObject> QuitButton = nullptr;
	
	public:
	int GetNumberOfPlayers();
	NDifficulty GetDifficulty();
	
	public:
	std::shared_ptr<TVisualObject> DifficultyLabel = nullptr;
	std::shared_ptr<TVisualObject> PlayersLabel = nullptr;
	
	protected:
	void OnQuitButton(std::shared_ptr<TVisualObject> obj);
	void OnSelectPlayers(std::shared_ptr<TVisualObject> obj);
	void OnSelectDifficulty(std::shared_ptr<TVisualObject> obj);
};


#endif //UNTITLED1_TSETTINGS_H
