//
// Created by choleraplague on 26.11.22.
//

#include "TVisualCard.h"

#define DECL(xx, type, prefix) \
	void TVisualCard::xx(type vv) { m_##prefix##xx = vv; } \
    type TVisualCard::xx() { return m_##prefix##xx; }
	
	DECL(DefaultY, int, i);
	DECL(DefaultX, int, i);
	DECL(ShiftX, int, i);
	DECL(ShiftY, int, i);
#undef DECL

void TVisualCard::Draw() {
	if(static_cast<int>(m_xState) & static_cast<int>(NState::Selected)) {
		Dx(DefaultX() + ShiftX());
		Dy(DefaultY() + ShiftY());
	} else {
		Dx(DefaultX());
		Dy(DefaultY());
	}
	TVisualObject::Draw();
}
