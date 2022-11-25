//
// Created by choleraplague on 14.11.22.
//

#include "TBoardController.h"
#include "../Config/TConfig.h"
#include "../Models/TBoardObject.h"


namespace Core {
	TBoardController::~TBoardController() {}
	
	TBoardController::TBoardController(const std::shared_ptr<TBoardObject>& board) { m_pBoard = board; }
	
	std::shared_ptr<TBoardObject>& TBoardController::Board() { return m_pBoard; }
	
	void TBoardController::OnEnterFrame() {
	
	}
	
	
} // Core