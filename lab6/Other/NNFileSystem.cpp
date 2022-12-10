//
// Created by choleraplague on 25.11.22.
//

#include "NNFileSystem.h"

static std::string bmp = ".bmp";

namespace NNFileSystem {
	std::string AssetsImagePath(std::string asset) {
		return "../Assets/" + asset + bmp;
	}
};