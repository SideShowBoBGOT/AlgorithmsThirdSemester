//
// Created by choleraplague on 26.11.22.
//

#include "TVisualCard.h"

void TVisualCard::Dx(int vv) {
	TParent::Dx(vv);
	Content->Dx(0);
}

#define DECL(xx, type, prefix) \
	void TVisualCard::xx(type vv) { m_##prefix##xx = vv; } \
    type TVisualCard::xx() { return m_##prefix##xx; }
	
	DECL(ShiftY, int, i);
#undef DECL

TVisualCard::TVisualCard() {
	Content = CreateObject<TControl>();
}

TVisualCard::~TVisualCard() {
	delete Content;
}

void TVisualCard::Render() {
	if(static_cast<int>(m_xState) & static_cast<int>(NState::OverSelected)) {
		Content->Dy(ShiftY());
	} else {
		Content->Dy(0);
	}
	TParent::Render();
}


