//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TMAINMENU_H
#define UNTITLED1_TMAINMENU_H

#include "../VisualModels/IVisual.h"
#include "../VisualModels/TVisualObject.h"

class TMainMenu : public IVisual {
	
	enum class Options {
		StartGame=0,
		Settings=1,
		Quit=2,
		Size=3
	};
	
	public:
	TMainMenu();
	virtual ~TMainMenu() override;
	
	public:
	virtual void HandleEvents() override;
	virtual void Clean() override;
	virtual void Render() override;
	
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
