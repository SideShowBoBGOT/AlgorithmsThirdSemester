//
// Created by choleraplague on 14.11.22.
//

#include "TInputHandler.h"
#include "../Config/TConfig.h"
#include "../Models/TBoardObject.h"


namespace Core {
	TInputHandler::~TInputHandler() {}
	
	TInputHandler::TInputHandler(const std::shared_ptr<TBoardObject>& board) { m_pBoard = board; }
	
	std::shared_ptr<TBoardObject>& TInputHandler::Board() { return m_pBoard; }
	
	void TInputHandler::OnEnterFrame() {
		}
	
	
} // Core