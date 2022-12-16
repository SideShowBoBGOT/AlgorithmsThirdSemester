//
// Created by choleraplague on 27.11.22.
//

#include "TAutoAlignArea.h"

TAutoAlignArea::TAutoAlignArea() {
	OnChangeHandler = [this](TControl* c) {
		ALignObjects();
	};
}

NAlign TAutoAlignArea::Align() { return m_xAlign; }
void TAutoAlignArea::Align(NAlign vv) {
	m_xAlign = vv;
}

void TAutoAlignArea::ALignObjects() {
	DetermineZIndexes();
	auto align = Align();
	switch(align) {
		case NAlign::Horizontal: AlignHorizontal(); break;
		case NAlign::Central : AlignCentral(); break;
		default: break;
	}
}

void TAutoAlignArea::AddChild(TControl* child) {
	TParent::AddChild(child);
	child->Dy(0);
	ALignObjects();
}

void TAutoAlignArea::DetermineZIndexes() {
	for(auto i=0;i<m_vObjectsPool.size();++i) {
		auto& obj = m_vObjectsPool[i];
		obj->ZIndex(i);
	}
}

void TAutoAlignArea::AlignHorizontal() {
	auto parts = CountVisibleChildren();
	if(not parts) return;
	auto unit = Width() / parts;
	for(auto i=0, j=0;i<m_vObjectsPool.size();++i) {
		auto& obj = m_vObjectsPool[i];
		if(obj->Visible()) {
			obj->Dx((unit - obj->Width()) / 2 + unit * j);\
			j++;
		}
		obj->ZIndex(i);
	}
}

int TAutoAlignArea::CountVisibleChildren() {
	auto parts = std::count_if(m_vObjectsPool.begin(), m_vObjectsPool.end(), [](TControl* ctrl) {
		return ctrl->Visible();
	});
	return int(parts);
}

void TAutoAlignArea::AlignCentral() {
	auto width = Width();
	auto total = 0;
	auto vis = std::vector<TControl*>();
	for(auto & obj : m_vObjectsPool) {
		if(obj->Visible()) {
			vis.emplace_back(obj);
			total += obj->Width();
		}
	}
	if(vis.empty()) return;
	
	auto parts = int(vis.size());
	vis[0]->Dx((total < width)?((width - total)/2):0);
	
	auto delta = 0.0;
	if(total < width) {
		delta = total;
	} else {
		auto r = double(width) / parts;
		auto rest = r * (parts - 1) + vis[parts-1]->Width();
		delta = (rest>width)?(2*width-rest):width;
	}
	delta /= parts;
	for(auto i=1;i<parts;++i) {
		vis[i]->Dx(vis[0]->Dx() + int(delta * i));\
	}
}