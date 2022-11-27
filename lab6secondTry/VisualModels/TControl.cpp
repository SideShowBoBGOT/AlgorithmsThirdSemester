//
// Created by choleraplague on 18.11.22.
//
#include <iostream>
#include "TControl.h"
#include "../GameSingletons/TTextureManager.h"
#include "../GameSingletons/TInputHandler.h"

#define DECL(xx, type, prefix) \
	void TControl::xx(type vv) {  \
		m_##prefix##xx = vv;          \
		if(m_pParent) m_pParent->OnChange(); \
		OnChange();\
	} \
    type TControl::xx() { return m_##prefix##xx; }
	
	DECL(Sx, int, i);
	DECL(Sy, int, i);
	DECL(Width, int, i);
	DECL(Height, int, i);
	DECL(Renderer, SDL_Renderer*, p);
	DECL(State, NState , x);
	DECL(UserData, std::string, s);
	DECL(Visible, bool, b);
	DECL(Enabled, bool, b);
	DECL(Parent, TControl*, p);
#undef DECL

void TControl::Dx(int vv) {
	m_iDx = m_pParent->Dx() + vv;
}

int TControl::Dx() { return m_iDx; }

void TControl::Dy(int vv) {
	m_iDy = m_pParent->Dy() + vv;
}

int TControl::Dy() { return m_iDy; }

#define INIT_BUTTON_HANDLER(xx) \
	void TControl::On##xx(std::function<void(TControl* obj)>&& func) { \
		On##xx##Handler = std::move(func);\
	} \
	void TControl::On##xx() { \
		if(On##xx##Handler) On##xx##Handler(this);\
	}
	
	INIT_BUTTON_HANDLER(LeftDown);
	INIT_BUTTON_HANDLER(RightDown);
	INIT_BUTTON_HANDLER(MiddleDown);
	INIT_BUTTON_HANDLER(LeftUp);
	INIT_BUTTON_HANDLER(RightUp);
	INIT_BUTTON_HANDLER(MiddleUp);
	INIT_BUTTON_HANDLER(Change);
#undef INIT_BUTTON_HANDLER

#define BOOL_STATE(xx) \
	void TControl::xx(bool vv) { \
        m_b##xx = vv;\
		if(m_b##xx) {\
			m_xState = static_cast<NState>(static_cast<int>(m_xState) | static_cast<int>(NState::xx));\
			return;\
		}\
		m_xState = static_cast<NState>(static_cast<int>(m_xState) & ~static_cast<int>(NState::xx));\
               \
    }\
	bool TControl::xx() { return m_b##xx; }

	BOOL_STATE(Selected);
	BOOL_STATE(Over);
#undef BOOL_STATE

TControl* TControl::GetThis() {
	return this;
}

void TControl::Render() {
	if(not Visible()) return;

	auto str = m_mMap[m_xState];
	if(str.empty()) {
		str = m_mMap[NState::Normal];
	}
	TTextureManager::Get()->Render(str.c_str(), m_iSx, m_iSy,
	 m_iDx, m_iDy, m_iWidth, m_iHeight, m_pRenderer);
}

void TControl::HandleEvents() {
	if(not Enabled()) return;
	
	auto x = TInputHandler::Get()->X();
	auto y = TInputHandler::Get()->Y();
	if( m_iDx <= x and x < m_iDx + m_iWidth
		and m_iDy <= y and y < m_iDy + m_iHeight ) {
		m_xState = static_cast<NState>(static_cast<int>(m_xState) | static_cast<int>(NState::Over));
		if(TInputHandler::Get()->Downs(NMouseButton::Left)) {
			Selected(not Selected());
		}
		#define INIT_BUTTON_HANDLER(button, type) \
			if(TInputHandler::Get()->type##s(NMouseButton::button)) {\
				On##button##type();\
			}
		
		INIT_BUTTON_HANDLER(Left, Down);
		INIT_BUTTON_HANDLER(Right, Down);
		INIT_BUTTON_HANDLER(Middle, Down);
		INIT_BUTTON_HANDLER(Left, Up);
		INIT_BUTTON_HANDLER(Right, Up);
		INIT_BUTTON_HANDLER(Middle, Up);
		#undef INIT_BUTTON_HANDLER
	} else {
		//m_xState = NState::Normal;
		m_xState = static_cast<NState>(static_cast<int>(m_xState) & ~static_cast<int>(NState::Over));
	}
}

void TControl::Clean() { return; }

void TControl::StateTexture(NState state, std::string textureID) { m_mMap[state] = textureID; }