//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TMAINMENU_H
#define UNTITLED1_TMAINMENU_H

#include "IGameState.h"
#include "TButton.h"

class TMainMenu : public IGameState {
	
	enum class Options {
		StartGame=0,
		Settings=1,
		Quit=2,
		Size=3
	};
	
	public:
	TMainMenu()=default;
	virtual ~TMainMenu()=default;
	
	public:
	virtual void HandleEvents();
	virtual void Update();
	virtual void Render();
	
	protected:
	
};


#endif //UNTITLED1_TMAINMENU_H
