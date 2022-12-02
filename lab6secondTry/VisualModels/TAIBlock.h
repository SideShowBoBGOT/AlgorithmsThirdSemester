//
// Created by choleraplague on 02.12.22.
//

#ifndef UNTITLED1_TAIBLOCK_H
#define UNTITLED1_TAIBLOCK_H

#include "TAutoAlignArea.h"

class TAIBlock : public TParent {
	public:
	TAIBlock()=default;
	TAIBlock(std::string name);
	virtual ~TAIBlock() override=default;
	
	public:
	TControl* Label = nullptr;
	TAutoAlignArea* LabelArea = nullptr;
	TAutoAlignArea* Cards = nullptr;
};


#endif //UNTITLED1_TAIBLOCK_H
