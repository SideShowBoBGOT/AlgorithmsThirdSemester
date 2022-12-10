//
// Created by choleraplague on 05.12.22.
//

#include <chrono>
#include "TAI.h"
#include "../../GameSingletons/TLogic.h"
#include "../Notify/SAIEndTurn.h"

void TAI::Move() {
	TLogic::Get()->AIThread = std::thread([this](){ DoMove(); });
}

void TAI::DoMove() {
	auto& ss = TLogic::Get()->Session;
	auto dur = std::chrono::seconds(2);
	std::this_thread::sleep_for(dur);
	
	ss->SendNotify(std::make_shared<SAIEndTurn>());
}
