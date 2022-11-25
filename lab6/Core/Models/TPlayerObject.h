//
// Created by choleraplague on 14.11.22.
//

#ifndef LAB6_TPLAYEROBJECT_H
#define LAB6_TPLAYEROBJECT_H

#include <vector>
#include <memory>
#include "TCardObject.h"

namespace Logic {
	
	class TPlayerObject {
		public:
		TPlayerObject()=default;
		virtual ~TPlayerObject()=default;
		
		virtual std::vector<std::shared_ptr<TCardObject>>& Cards();
		protected:
		std::vector<std::shared_ptr<TCardObject>> m_vCards;
	};
	
} // Logic

#endif //LAB6_TPLAYEROBJECT_H
