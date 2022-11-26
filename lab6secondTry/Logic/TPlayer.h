//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TPLAYER_H
#define UNTITLED1_TPLAYER_H

#include <vector>
#include <memory>
#include "TCard.h"

class TPlayer {
	public:
	TPlayer()=default;
	virtual ~TPlayer()=default;
	
	virtual std::vector<std::shared_ptr<TCard>>& Cards();
	virtual bool IsActive();
	virtual void IsActive(bool vv);
	
	protected:
	std::vector<std::shared_ptr<TCard>> m_vCards;
	bool m_bIsActive = false;
};


#endif //UNTITLED1_TPLAYER_H
