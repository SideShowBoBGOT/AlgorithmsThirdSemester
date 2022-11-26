//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TVISUALOBJECT_H
#define UNTITLED1_TVISUALOBJECT_H

#include <functional>
#include <string>
#include <map>
#include <SDL2/SDL.h>
#include "IVisual.h"

class TVisualObject : public IVisual {
	public:
	TVisualObject()=default;
	virtual ~TVisualObject()=default;
	
	public:
	virtual void HandleEvents();
	virtual void Render();
	virtual void Clean();
	
	public:
	enum class NState {
		Normal=0,
		Selected=1,
		Over=2,
		OverSelected=Selected | Over,
	};
	
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
	#undef DECL
	
	#define INIT_HANDLER(button, type) \
    	private:\
		std::function<void(TVisualObject* obj)> On##button##type##Handler = nullptr; \
                                    \
		public:\
		virtual void On##button##type(std::function<void(TVisualObject* obj)> func) { \
        	On##button##type##Handler = func;\
		} \
		virtual void On##button##type() { \
			if(On##button##type##Handler) On##button##type##Handler(this);\
		}
		
		INIT_HANDLER(Left, Down);
		INIT_HANDLER(Right, Down);
		INIT_HANDLER(Middle, Down);
		INIT_HANDLER(Left, Up);
		INIT_HANDLER(Right, Up);
		INIT_HANDLER(Middle, Up);
	#undef INIT_HANDLER
	
	public:
	virtual void StateTexture(NState state, std::string str);
	
	protected:
	std::map<NState, std::string> m_mMap = 	{
												{NState::Normal, ""},
												{NState::Over, ""},
												{NState::OverSelected, ""},
												{NState::Selected, ""}
											};
};
using NState = TVisualObject::NState;

#endif //UNTITLED1_TVISUALOBJECT_H
