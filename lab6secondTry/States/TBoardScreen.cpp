//
// Created by choleraplague on 25.11.22.
//

#include "TBoardScreen.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../Other/NNFileSystem.h"

static int constexpr s_iObjectWidth = 158;
static int constexpr s_iObjectHeight = 35;
static const std::string s_sBoardScreenPath = "BoardScreen/";

TBoardScreen::TBoardScreen() {
	auto width = TGame::Get()->ScreenWidth();
	auto height = TGame::Get()->ScreenHeight();
	
	#define INIT(xx, parent, type, dx, dy, sx, sy, w, h, func) \
		xx = parent->CreateObject<type>();\
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
	
		INIT(ButtonPanel, this, TAutoAlignArea, 0, height - s_iObjectHeight, 0, 0, width, s_iObjectHeight, [](TControl* obj) {});
		INIT(AIPanel, this, TAutoAlignArea, 0, 0, 0, 0, width, s_iObjectHeight, [](TControl* obj) {});

		#define INIT_BUTTON(xx) \
				INIT(xx, ButtonPanel, TControl, 0, 0, 0, 0, s_iObjectWidth, s_iObjectHeight, [this](TControl* obj) { On##xx(obj); });\
			
				INIT_BUTTON(NextPageButton);
				INIT_BUTTON(PrevPageButton);
				INIT_BUTTON(TakeButton);
				INIT_BUTTON(PutButton);
				INIT_BUTTON(EndTurnButton);
				INIT_BUTTON(DeselectButton);
				INIT_BUTTON(QuitButton);
		#undef INIT_BUTTON
		
		#define INIT_LABEL(xx) \
			INIT(xx, AIPanel, TControl, 0, 0, 0, 0, s_iObjectWidth, s_iObjectHeight, [this](TControl* obj) {}); \
		
			INIT_LABEL(AIOneLabel);
			INIT_LABEL(AITwoLabel);
			INIT_LABEL(AIThreeLabel);
		#undef INIT_LABEL
	
	#undef INIT
	
	ButtonPanel->ALignObjects();
	AIPanel->ALignObjects();
}

void TBoardScreen::OnQuitButton(TControl* obj) {
	TGameStateMachine::Get()->PopState();
}

void TBoardScreen::OnPutButton(TControl* obj) {

}

void TBoardScreen::OnTakeButton(TControl* obj) {

}

void TBoardScreen::OnEndTurnButton(TControl* obj) {

}

void TBoardScreen::OnNextPageButton(TControl* obj) {

}

void TBoardScreen::OnPrevPageButton(TControl* obj) {

}

void TBoardScreen::OnDeselectButton(TControl* obj) {

}

void TBoardScreen::LockInterface() {
	for(auto& obj : m_vObjectsPool) {
		obj->Enabled(false);
	}
}

void TBoardScreen::UnLockInterface() {
	for(auto& obj : m_vObjectsPool) {
		obj->Enabled(true);
	}
}
