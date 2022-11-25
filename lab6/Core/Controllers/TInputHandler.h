//
// Created by choleraplague on 14.11.22.
//

#ifndef LAB6_TBOARDCONTROLLER_H
#define LAB6_TBOARDCONTROLLER_H

#include "../Models/TBoardObject.h"
#include "../Patterns/TSingleton.h"

namespace Core {
	
	class TBoardController : public TSingleton<TBoardController> {
		public:
		TBoardController()=default;
		TBoardController(const std::shared_ptr<TBoardObject>& board);
		virtual ~TBoardController() override;
		
		public:
		virtual void OnEnterFrame();
		
		public:
		std::shared_ptr<TBoardObject>& Board();
		
		protected:
		std::shared_ptr<TBoardObject> m_pBoard = nullptr;
	};
	
} // Core

#endif //LAB6_TBOARDCONTROLLER_H
