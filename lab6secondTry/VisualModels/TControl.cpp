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
	DECL(ZIndex, int, i);
#undef DECL

void TControl::Dx(int vv) {
	m_iDx = vv;
}

int TControl::Dx() { return m_iDx; }

void TControl::Dy(int vv) {
	m_iDy = vv;
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
			State(static_cast<NState>(static_cast<int>(State()) | static_cast<int>(NState::xx)));\
			return;\
		}\
		State(static_cast<NState>(static_cast<int>(State()) & ~static_cast<int>(NState::xx)));\
               \
    }\
	bool TControl::xx() { return m_b##xx; }

	BOOL_STATE(Selected);
	BOOL_STATE(Over);
#undef BOOL_STATE

bool TControl::IsMouseOn() {
	auto x = TInputHandler::Get()->X();
	auto y = TInputHandler::Get()->Y();
	auto dx = AbsoluteDx();
	auto dy = AbsoluteDy();
	return dx <= x and x < dx + m_iWidth
		and dy <= y and y < dy + m_iHeight;
}


bool TControl::Render() {
	if(not Visible()) return false;

	auto str = m_mMap[m_xState];
	if(str.empty()) {
		str = m_mMap[NState::Normal];
	}
	auto dx = AbsoluteDx();
	auto dy = AbsoluteDy();
	TTextureManager::Get()->Render(str.c_str(), m_iSx, m_iSy,
	 dx, dy, m_iWidth, m_iHeight, m_pRenderer);
	return true;
}

bool TControl::HandleEvents() {
	if(not Enabled()) return false;
	
	Over(IsMouseOn());
	
	if(IsMouseOn()) {
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
	}
	return true;
}

bool TControl::Clean() { return true; }

void TControl::StateTexture(NState state, std::string textureID) { m_mMap[state] = textureID; }

int TControl::AbsoluteDx() {
	if(not m_pParent) return 0;
	return m_iDx + m_pParent->AbsoluteDx();
}

int TControl::AbsoluteDy() {
	if(not m_pParent) return 0;
	return m_iDy + m_pParent->AbsoluteDy();
}

