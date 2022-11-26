//
// Created by choleraplague on 14.11.22.
//

#include "TPlayerObject.h"

namespace Core {
	std::vector<std::shared_ptr<TCardObject>>& TPlayerObject::Cards() {
		return m_vCards;
	}
} // Core