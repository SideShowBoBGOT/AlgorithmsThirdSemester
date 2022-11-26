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
	TVisualObject* StartButton = nullptr;
	TVisualObject* SettingsButton = nullptr;
	TVisualObject* QuitButton = nullptr;
	
	protected:
	void OnStartButton(TVisualObject* obj);
	void OnSettigsButton(TVisualObject* obj);
	void OnQuitButton(TVisualObject* obj);
};


#endif //UNTITLED1_TMAINMENU_H
