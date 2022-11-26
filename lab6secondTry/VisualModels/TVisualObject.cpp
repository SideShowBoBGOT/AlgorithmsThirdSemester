//
// Created by choleraplague on 18.11.22.
//
#include <iostream>
#include "TVisualObject.h"
#include "../GameSingletons/TTextureManager.h"
#include "../GameSingletons/TInputHandler.h"

#define DECL(xx, type, prefix) \
	void TVisualObject::xx(type vv) { m_##prefix##xx = vv; } \
    type TVisualObject::xx() { return m_##prefix##xx; }
	
	DECL(Dx, int, i);
	DECL(Dy, int, i);
	DECL(Sx, int, i);
	DECL(Sy, int, i);
	DECL(Width, int, i);
	DECL(Height, int, i);
	DECL(Renderer, SDL_Renderer*, p);
	DECL(State, NState , x);
	DECL(UserData, std::string, s);
#undef DECL


#define INIT_HANDLER(button, type) \
	void TVisualObject::On##button##type(std::function<void(std::shared_ptr<TVisualObject> obj)>&& func) { \
		On##button##type##Handler = std::move(func);\
	} \
	void TVisualObject::On##button##type() { \
		if(On##button##type##Handler) On##button##type##Handler(GetThis());\
	}
	
	INIT_HANDLER(Left, Down);
	INIT_HANDLER(Right, Down);
	INIT_HANDLER(Middle, Down);
	INIT_HANDLER(Left, Up);
	INIT_HANDLER(Right, Up);
	INIT_HANDLER(Middle, Up);
#undef INIT_HANDLER

#define BOOL_STATE(xx) \
	void TVisualObject::xx(bool vv) { \
        m_b##xx = vv;\
		if(m_b##xx) {\
			m_xState = static_cast<NState>(static_cast<int>(m_xState) | static_cast<int>(NState::xx));\
			return;\
		}\
		m_xState = static_cast<NState>(static_cast<int>(m_xState) & ~static_cast<int>(NState::xx));\
               \
    }\
	bool TVisualObject::xx() { return m_b##xx; }

	BOOL_STATE(Selected);
	BOOL_STATE(Over);
#undef BOOL_STATE

std::shared_ptr<TVisualObject> TVisualObject::GetThis() {
	return std::shared_ptr<TVisualObject>(this);
}

void TVisualObject::Render() {
	auto str = m_mMap[m_xState];
	if(str.empty()) {
		str = m_mMap[NState::Normal];
	}
	TTextureManager::Get()->Render(str.c_str(), m_iSx, m_iSy,
	 m_iDx, m_iDy, m_iWidth, m_iHeight, m_pRenderer);
}

void TVisualObject::HandleEvents() {
	auto x = TInputHandler::Get()->X();
	auto y = TInputHandler::Get()->Y();
	if( m_iDx <= x and x < m_iDx + m_iWidth
		and m_iDy <= y and y < m_iDy + m_iHeight ) {
		m_xState = static_cast<NState>(static_cast<int>(m_xState) | static_cast<int>(NState::Over));
		if(TInputHandler::Get()->Downs(NMouseButton::Left)) {
			Selected(not Selected());
		}
		#define INIT_HANDLER(button, type) \
			if(TInputHandler::Get()->type##s(NMouseButton::button)) {\
				On##button##type();\
			}
		
		INIT_HANDLER(Left, Down);
		INIT_HANDLER(Right, Down);
		INIT_HANDLER(Middle, Down);
		INIT_HANDLER(Left, Up);
		INIT_HANDLER(Right, Up);
		INIT_HANDLER(Middle, Up);
		#undef INIT_HANDLER
	} else {
		//m_xState = NState::Normal;
		m_xState = static_cast<NState>(static_cast<int>(m_xState) & ~static_cast<int>(NState::Over));
	}
}

void TVisualObject::Clean() { return; }

void TVisualObject::StateTexture(NState state, std::string textureID) { m_mMap[state] = textureID; }