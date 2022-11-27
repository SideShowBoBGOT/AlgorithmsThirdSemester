//
// Created by choleraplague on 27.11.22.
//

#include "TAutoAlignArea.h"

NAlign TAutoAlignArea::Align() { return m_xAlign; }
void TAutoAlignArea::Align(NAlign vv) {
	m_xAlign = vv;
	ALignObjects();
}

void TAutoAlignArea::ALignObjects() {
	#define ALIGN(xx, yy) \
		auto parts = std::count_if(m_vObjectsPool.begin(), m_vObjectsPool.end(), [](TControl* ctrl) {\
			return ctrl->Visible();\
		});                  \
        if(parts==0) return;               \
		auto unit##yy = yy() / parts;\
		for(auto i=0, j=0;i<m_vObjectsPool.size();++i) {\
			auto& obj = m_vObjectsPool[i];\
			if(obj->Visible()) {\
				obj->D##xx(unit##yy / 2 - obj->yy() / 2 + unit##yy * j);\
				j++;\
			}\
		}
	
		if(Align()==NAlign::Horizontal) {
			ALIGN(x, Width);
		} else if(Align()==NAlign::Vertical) {
			ALIGN(y, Height);
		}
	
	#undef ALIGN
}
