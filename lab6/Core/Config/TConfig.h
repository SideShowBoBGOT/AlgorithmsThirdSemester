//
// Created by choleraplague on 14.11.22.
//

#ifndef LAB6_TCONFIG_H
#define LAB6_TCONFIG_H

#include "../Patterns/TSingleton.h"

namespace Core {
	
	class TConfig : public TSingleton<TConfig> {
		public:
		TConfig();
		virtual ~TConfig()=default;
		
		public:
		virtual int TotalCards();
		virtual int CardsPerPlayer();
		
		protected:
		virtual void ReadConfig();

		protected:
		int m_iTotalCards = 52;
		int m_iCardsPerPlayer = 4;
	};
	
} // Core

#endif //LAB6_TCONFIG_H
