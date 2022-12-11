//
// Created by choleraplague on 25.11.22.
//

#ifndef UNTITLED1_NNFILESYSTEM_H
#define UNTITLED1_NNFILESYSTEM_H

#include <string>

static std::string bmp = ".bmp";

namespace NNFileSystem {
	inline std::string AssetsImagePath(const std::string& asset) {
		return "../Assets/" + asset + bmp;
	}
}

#endif //UNTITLED1_NNFILESYSTEM_H
