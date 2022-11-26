//
// Created by choleraplague on 25.11.22.
//

#include "TBoardScreen.h"

TBoardScreen::TBoardScreen() {
	UserPanel = new TPanel();
}

TBoardScreen::~TBoardScreen() {
	delete UserPanel;
}

void TBoardScreen::HandleEvents() {
	UserPanel->HandleEvents();
}

void TBoardScreen::Clean() {
	UserPanel->Clean();
}

void TBoardScreen::Render() {
	UserPanel->Draw();
}