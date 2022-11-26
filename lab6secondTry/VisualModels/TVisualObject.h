//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TVISUALOBJECT_H
#define UNTITLED1_TVISUALOBJECT_H

#include <functional>
#include <string>
#include <map>
#include <memory>
#include <SDL2/SDL.h>
#include "IVisual.h"

class TVisualObject : public IVisual {
	public:
	enum class NState {
		Normal=0,
		Selected=1,
		Over=2,
		OverSelected=Selected | Over,
	};

	public:
	TVisualObject()=default;
	virtual ~TVisualObject() override=default;
	
	public:
	virtual void HandleEvents() override;
	virtual void Render() override;
	virtual void Clean() override;
	
	public:
	virtual void StateTexture(NState state, std::string str);
	
	public:
	virtual TVisualObject* GetThis();
	
	
	
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
	#undef DECL
	
	#define INIT_HANDLER(button, type) \
    	private:\
		std::function<void(TVisualObject* obj)> On##button##type##Handler = nullptr; \
                                    \
		public:\
		virtual void On##button##type(std::function<void(TVisualObject* obj)>&& func);\
		virtual void On##button##type();
		
		INIT_HANDLER(Left, Down);
		INIT_HANDLER(Right, Down);
		INIT_HANDLER(Middle, Down);
		INIT_HANDLER(Left, Up);
		INIT_HANDLER(Right, Up);
		INIT_HANDLER(Middle, Up);
	#undef INIT_HANDLER
	
	
	
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
