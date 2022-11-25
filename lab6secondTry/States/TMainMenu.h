//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TMAINMENU_H
#define UNTITLED1_TMAINMENU_H

#include "../Models/TGameState.h"
#include "../Models/TVisualObject.h"

class TMainMenu : public IGameState {
	
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
	
	protected:
	void OnStartButton(TVisualObject* obj);
	void OnSettigsButton(TVisualObject* obj);
	void OnQuitButton(TVisualObject* obj);
	
	protected:
	TVisualObject* m_pStartButton = nullptr;
	TVisualObject* m_pSettingsButton = nullptr;
	TVisualObject* m_pQuitButton = nullptr;
};


#endif //UNTITLED1_TMAINMENU_H
