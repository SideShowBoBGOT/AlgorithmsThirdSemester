//
// Created by choleraplague on 27.11.22.
//

#include "TAutoAlignArea.h"

TAutoAlignArea::TAutoAlignArea() {
	OnChangeHandler = [this](TControl* c) {
		ALignObjects();
	};
}

int TAutoAlignArea::Padding() { return m_iPadding; }
void TAutoAlignArea::Padding(int vv) {
	m_iPadding = vv;
}

NAlign TAutoAlignArea::Align() { return m_xAlign; }
void TAutoAlignArea::Align(NAlign vv) {
	m_xAlign = vv;
}

void TAutoAlignArea::ALignObjects() {
	for(auto i=0;i<m_vObjectsPool.size();++i) {
		auto& obj = m_vObjectsPool[i];
		obj->ZIndex(i);
	}
	#define ALIGN(xx, yy) \
		auto parts = std::count_if(m_vObjectsPool.begin(), m_vObjectsPool.end(), [](TControl* ctrl) {\
			return ctrl->Visible();\
		});                  \
        if(parts==0) return;               \
		auto unit = (yy() - Padding()) / parts;\
		for(auto i=0, j=0;i<m_vObjectsPool.size();++i) {\
			auto& obj = m_vObjectsPool[i];\
			if(obj->Visible()) {\
				obj->D##xx((unit - obj->yy() + Padding()) / 2 + unit * j);\
				j++;\
			}                   \
        	obj->ZIndex(i);               \
		}
	
	#define ALIGN_CENTRAL_MIDDLE(xx, yy) \
        auto total = 0;           \
        auto vis = std::vector<TControl*>();                              \
    	for(auto i=0;i<m_vObjectsPool.size();++i) { \
			auto& obj = m_vObjectsPool[i];\
			if(obj->Visible()) {               \
				vis.emplace_back(obj);\
            	total += obj->yy();                         \
			}                             \
		}                                   \
		if(vis.empty()) return;             \
                                      \
        auto parts = static_cast<int>(vis.size());                              \
		vis[0]->D##xx((total < yy())?((yy() - total)/2):0);                \
                                      \
		auto delta = 0.0;                     \
		if(total < yy()) {                  \
            delta = total;            \
		} else {                            \
            auto r = double(yy()) / parts;                          \
			auto rest = r * (parts - 1) + vis[parts-1]->yy();          \
        	delta = (rest>yy())?(2*yy()-rest):yy();               \
        }                             \
		delta /= parts;\
		for(auto i=1;i<parts;++i) {   \
			vis[i]->D##xx(vis[0]->D##xx() + delta * i);	                        \
		}
	
		auto align = Align();
		switch(align) {
			case NAlign::Horizontal: {
				ALIGN(x, Width);
				break;
			} case NAlign::Vertical: {
				ALIGN(y, Height);
				break;
			} case NAlign::Central : {
				ALIGN_CENTRAL_MIDDLE(x, Width);
				break;
			} case NAlign::Middle : {
				ALIGN_CENTRAL_MIDDLE(y, Height);
				break;
			}
		}
	
	#undef ALIGN
	#undef ALIGN_CENTRAL_MIDDLE
}

void TAutoAlignArea::OnChange(std::function<void(TControl*)>&& func) {
	OnChangeHandler = [this, f = std::move(func)](TControl* obj) {
		f(obj);
		ALignObjects();
	};
}
