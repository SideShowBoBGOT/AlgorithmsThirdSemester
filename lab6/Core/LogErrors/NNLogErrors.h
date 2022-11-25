//
// Created by choleraplague on 15.11.22.
//

#ifndef LAB6_NNLOGERRORS_H
#define LAB6_NNLOGERRORS_H

namespace Logic {
	namespace NNLogErrors {
		#define _ERROR(x) throw x
		#define _LOG(x) printf_s(x)
		#define TO_STR(x) #x
	}
}

#endif //LAB6_NNLOGERRORS_H
