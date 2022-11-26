//
// Created by choleraplague on 25.11.22.
//

#include "TBoardScreen.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../Other/NNFileSystem.h"

static int constexpr s_iObjectWidth = 158;
static int constexpr s_iObjectHeigth = 35;
static const std::string s_sBoardScreenPath = "BoardScreen/";

TBoardScreen::TBoardScreen() {
	auto width = TGame::Get()->ScreenWidth();
	auto heigth = TGame::Get()->ScreenHeight();
	
	#define INIT(xx, dx, dy, sx, sy, w, h, func) \
		xx = CreateObject();\
		xx->Dx(dx);\
		xx->Dy(dy);\
		xx->Sx(sx);\
		xx->Sx(sy);\
		xx->Width(w);\
		xx->Height(h);\
		xx->Renderer(TGame::Get()->Renderer()); \
		TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"Normal").c_str(), #xx"Normal", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"Over").c_str(), #xx"Over", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"Selected").c_str(), #xx"Selected", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load(NNFileSystem::AssetsImagePath(s_sBoardScreenPath + #xx+"/"+"OverSelected").c_str(), #xx"OverSelected", TGame::Get()->Renderer());\
		xx->StateTexture(NState::Normal, #xx"Normal");\
		xx->StateTexture(NState::Over, #xx"Over");\
		xx->StateTexture(NState::Selected, #xx"Selected");\
		xx->StateTexture(NState::OverSelected, #xx"OverSelected");\
		xx->OnLeftDown(func);
	
	auto i = 0;
	#define INIT_BUTTON(xx, dx, dy) \
        INIT(xx, dx, dy, 0, 0, s_iObjectWidth, s_iObjectHeigth, [this](std::shared_ptr<TVisualObject> obj) { On##xx(obj); });\
		++i;
	
		INIT_BUTTON(NextPageButton, s_iObjectWidth * i, heigth - s_iObjectHeigth);
		INIT_BUTTON(PrevPageButton, s_iObjectWidth * i, heigth - s_iObjectHeigth);
		INIT_BUTTON(TakeButton, s_iObjectWidth * i, heigth - s_iObjectHeigth);
		INIT_BUTTON(PutButton, s_iObjectWidth * i, heigth - s_iObjectHeigth);
		INIT_BUTTON(EndTurnButton, s_iObjectWidth * i, heigth - s_iObjectHeigth);
		INIT_BUTTON(DeselectButton, s_iObjectWidth * i, heigth - s_iObjectHeigth);
		INIT_BUTTON(QuitButton, width - s_iObjectWidth, heigth - s_iObjectHeigth);
	#undef INIT_BUTTON
	
	auto j = 0;
	#define INIT_LABEL(xx, dx, dy) \
		INIT(xx, dx, dy, 0, 0, s_iObjectWidth, s_iObjectHeigth, [this](std::shared_ptr<TVisualObject> obj) {}); \
		j++;
	
		INIT_LABEL(AIOneLabel, s_iObjectWidth * j, 0);
		INIT_LABEL(AITwoLabel, s_iObjectWidth * j, 0);
		INIT_LABEL(AIThreeLabel, s_iObjectWidth * j, 0);
	#undef INIT_LABEL
	
	#undef INIT
}

void TBoardScreen::OnQuitButton(std::shared_ptr<TVisualObject> obj) {
	TGameStateMachine::Get()->PopState();
}

void TBoardScreen::OnPutButton(std::shared_ptr<TVisualObject> obj) {

}

void TBoardScreen::OnTakeButton(std::shared_ptr<TVisualObject> obj) {

}

void TBoardScreen::OnEndTurnButton(std::shared_ptr<TVisualObject> obj) {

}

void TBoardScreen::OnNextPageButton(std::shared_ptr<TVisualObject> obj) {

}

void TBoardScreen::OnPrevPageButton(std::shared_ptr<TVisualObject> obj) {

}

void TBoardScreen::OnDeselectButton(std::shared_ptr<TVisualObject> obj) {

}
