//
// Created by choleraplague on 15.11.22.
//

#ifndef LAB6_NNGAMESETTIGS_H
#define LAB6_NNGAMESETTIGS_H

namespace Logic {
	namespace NNGameSettings {
		enum class NGameSettings {
			Difficulty=0,
			TotalPlayers=1,
			Size=2
		};
	}
	typedef NNGameSettings::NGameSettings NGameSettings;
}

#endif //LAB6_NNGAMESETTIGS_H
