//
// Created by choleraplague on 26.11.22.
//

#include "TPlayer.h"

std::vector<std::shared_ptr<TCard>>& TPlayer::Cards() {
	return m_vCards;
}

bool TPlayer::IsActive() { return m_bIsActive; }

void TPlayer::IsActive(bool vv) { m_bIsActive = vv; }
