//
// Created by choleraplague on 14.11.22.
//

#ifndef LAB6_TINPUTHANDLER_H
#define LAB6_TINPUTHANDLER_H

#include "../Patterns/TSingleton.h"

namespace Core {
	
	class TInputHandler : public TSingleton<TInputHandler> {
		public:
		TInputHandler()=default;
		virtual ~TInputHandler() override;
		
		public:
		void LockUserInput();
		void UnlockUserInput();
		
		public:
		void
		bool OnMouseRight();
		
		public:
		
		protected:
		bool m_bIsUserInput = true;
	};
	
	
} // Core

#endif //LAB6_TINPUTHANDLER_H
