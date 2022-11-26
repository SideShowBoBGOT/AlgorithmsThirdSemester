//
// Created by choleraplague on 25.11.22.
//

#include "TBoardScreen.h"
#include "../GameSingletons/TGame.h"
#include "../Other/NNFileSystem.h"

static int constexpr s_iButtonWidth = 158;
static int constexpr s_iButtonHeight = 35;

TBoardScreen::TBoardScreen() {
	auto width = TGame::Get()->ScreenWidth();
	auto heigth = TGame::Get()->ScreenHeight();
	

}

TBoardScreen::~TBoardScreen() {
}

void TBoardScreen::HandleEvents() {
}

void TBoardScreen::Clean() {
}

void TBoardScreen::Render() {
}