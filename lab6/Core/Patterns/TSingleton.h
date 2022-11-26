//
// Created by choleraplague on 18.11.22.
//

#include "TMainMenu.h"
#include "../GameSingletons/TGame.h"
#include "../GameSingletons/TTextureManager.h"
#include "../Models/TVisualObject.h"

#define DECL(xx, type, prefix, value) \
	static const type s_##prefix##xx = value;
	DECL(Path, std::string, s, "../Assets/MainMenu/");
	DECL(Type, std::string, s, ".bmp");
	DECL(ButtonWidth, int, i, 158);
	DECL(ButtonHeight, int, i, 35);
#undef DECL

TMainMenu::TMainMenu() {
	#define INIT_BUTTON(xx, dx, dy) \
    	m_p##xx = new TVisualObject();\
		m_p##xx->Dx(dx);\
		m_p##xx->Dy(dy);\
		m_p##xx->Sx(0);\
		m_p##xx->Sx(0);\
		m_p##xx->Width(s_iButtonWidth);\
		m_p##xx->Height(s_iButtonHeight);\
		m_p##xx->Renderer(TGame::Get()->Renderer()); \
        TTextureManager::Get()->Load((s_sPath+#xx+"/"+"Normal"+s_sType).c_str(), #xx"Normal", TGame::Get()->Renderer());\
        TTextureManager::Get()->Load((s_sPath+#xx+"/"+"Over"+s_sType).c_str(), #xx"Over", TGame::Get()->Renderer());\
		m_p##xx->StateTexture(NState::Normal, #xx"Normal");\
		m_p##xx->StateTexture(NState::Over, #xx"Over");\
	
		INIT_BUTTON(StartButton, 241, 200);
		INIT_BUTTON(SettingsButton, 241, 235);
		INIT_BUTTON(QuitButton, 241, 270);
	#undef INIT_BUTTON
}

TMainMenu::~TMainMenu() {
	delete m_pStartButton;
	delete m_pSettingsButton;
	delete m_pQuitButton;
}

void TMainMenu::HandleEvents() {
	m_pStartButton->HandleEvents();
	m_pSettingsButton->HandleEvents();
	m_pQuitButton->HandleEvents();
}

void TMainMenu::Clean() {
	m_pStartButton->Clean();
	m_pSettingsButton->Clean();
	m_pQuitButton->Clean();
}

void TMainMenu::Render() {
	m_pStartButton->Draw();
	m_pSettingsButton->Draw();
	m_pQuitButton->Draw();
}


