//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TMAINMENU_H
#define UNTITLED1_TMAINMENU_H

#include "../VisualModels/TParent.h"

class TMainMenu : public TParent {
	public:
	TMainMenu();
	virtual ~TMainMenu() override=default;
	
	public:
	TControl* StartButton = nullptr;
	TControl* SettingsButton = nullptr;
	TControl* QuitButton = nullptr;
	
	protected:
	void OnStartButton(TControl* obj);
	void OnSettigsButton(TControl* obj);
	void OnQuitButton(TControl* obj);
};


#endif //UNTITLED1_TMAINMENU_H
