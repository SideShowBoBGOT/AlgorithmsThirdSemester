//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TMAINMENU_H
#define UNTITLED1_TMAINMENU_H

#include "../VisualModels/TVisualParent.h"

class TMainMenu : public TVisualParent {
	public:
	TMainMenu();
	virtual ~TMainMenu() override=default;
	
	public:
	std::shared_ptr<TVisualObject> StartButton = nullptr;
	std::shared_ptr<TVisualObject> SettingsButton = nullptr;
	std::shared_ptr<TVisualObject> QuitButton = nullptr;
	
	protected:
	void OnStartButton(std::shared_ptr<TVisualObject> obj);
	void OnSettigsButton(std::shared_ptr<TVisualObject> obj);
	void OnQuitButton(std::shared_ptr<TVisualObject> obj);
};


#endif //UNTITLED1_TMAINMENU_H
