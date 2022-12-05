//
// Created by choleraplague on 05.12.22.
//

#include "TTimer.h"

void TTimer::Start() {
	m_xStartTime = std::chrono::system_clock::now();
	m_bRunning = true;
}

void TTimer::Stop() {
	m_xEndTime = std::chrono::system_clock::now();
	m_bRunning = false;
}

double TTimer::ElapsedMilliseconds() {
	std::chrono::time_point<std::chrono::system_clock> endTime;
	endTime = (m_bRunning) ? std::chrono::system_clock::now() : m_xEndTime;
	return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_xStartTime).count();
}

bool TTimer::IsRunning() {
	return m_bRunning;
}
