//
// Created by choleraplague on 26.11.22.
//

#ifndef UNTITLED1_TAIPLAYERSPANEL_H
#define UNTITLED1_TAIPLAYERSPANEL_H

#include "../Models/TVisualObject.h"

class TAIPlayersPanel : TVisualObject {
	public:
	TAIPlayersPanel();
	virtual ~TAIPlayersPanel() override;
	
	public:
	virtual void HandleEvents() override;
	virtual void Draw() override;
	virtual void Clean() override;
	
	#define DECL(xx, type, prefix, val) \
		protected:            \
		type m_##prefix##xx = val;      \
                                \
        public:                        \
		type xx();\
		void xx(type vv);
	
		DECL(PlayersNumber, int, i, 2);
	#undef DECL
	
	
	public:
	TVisualObject* AIPlayers[3] = { nullptr };
};


#endif //UNTITLED1_TAIPLAYERSPANEL_H
