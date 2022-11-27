//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TCONTROL_H
#define UNTITLED1_TCONTROL_H

#include <functional>
#include <string>
#include <map>
#include <memory>
#include <SDL2/SDL.h>
#include "IControl.h"
#include "Enums/NNState.h"

class TControl : public IControl {
	public:
	TControl()=default;
	virtual ~TControl() override=default;
	
	public:
	virtual void HandleEvents() override;
	virtual void Render() override;
	virtual void Clean() override;
	
	public:
	virtual void StateTexture(NState state, std::string str);
	
	public:
	virtual TControl* GetThis();
	
	
	
	#define DECL(xx, type, prefix, val) \
		protected:            \
		type m_##prefix##xx = val;      \
                                \
        public:                        \
		virtual void xx(type vv); \
		virtual type xx();
	
		DECL(Dx, int, i, 0);
		DECL(Dy, int, i, 0);
		DECL(Sx, int, i, 0);
		DECL(Sy, int, i, 0);
		DECL(Width, int, i, 0);
		DECL(Height, int, i, 0);
		DECL(Renderer, SDL_Renderer*, p, nullptr);
		DECL(State, NState, x, NState::Normal);
		DECL(Selected, bool, b, false);
		DECL(Over, bool, b, false);
		DECL(UserData, std::string, s, "");
		DECL(Enabled, bool, b, true);
		DECL(Visible, bool, b, true);
		DECL(Parent, TControl*, p, nullptr);
	#undef DECL
	
	#define INIT_BUTTON_HANDLER(button, type) \
    	private:\
		std::function<void(TControl* obj)> On##button##type##Handler = nullptr; \
                                    \
		public:\
		virtual void On##button##type(std::function<void(TControl* obj)>&& func);\
		virtual void On##button##type();
		
		INIT_BUTTON_HANDLER(Left, Down);
		INIT_BUTTON_HANDLER(Right, Down);
		INIT_BUTTON_HANDLER(Middle, Down);
		INIT_BUTTON_HANDLER(Left, Up);
		INIT_BUTTON_HANDLER(Right, Up);
		INIT_BUTTON_HANDLER(Middle, Up);
	#undef INIT_BUTTON_HANDLER
	
	
	
	protected:
	std::map<NState, std::string> m_mMap = 	{
												{NState::Normal, ""},
												{NState::Over, ""},
												{NState::OverSelected, ""},
												{NState::Selected, ""}
											};
};
#endif //UNTITLED1_TCONTROL_H
