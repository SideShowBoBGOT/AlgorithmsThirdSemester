//
// Created by choleraplague on 27.11.22.
//

#ifndef UNTITLED1_NNSTATE_H
#define UNTITLED1_NNSTATE_H

namespace NNState {
	enum class NState {
		Normal=0,
		Selected=1,
		Over=2,
		OverSelected=Selected | Over,
	};
}
typedef NNState::NState NState;
#endif //UNTITLED1_NNSTATE_H
