//
// Created by choleraplague on 14.11.22.
//

#include <fstream>

#include "TConfig.h"

namespace Logic {
	
	static const std::string s_sConfigPath = "";
	
	TConfig::TConfig() { ReadConfig(); }
	
	int TConfig::TotalCards() { return m_iTotalCards; }
	int TConfig::CardsPerPlayer() { return m_iCardsPerPlayer; }
	
	void TConfig::ReadConfig() {
	
	}
} // Logic