//
// Created by choleraplague on 15.11.22.
//

#ifndef LAB6_TSINGLETON_H
#define LAB6_TSINGLETON_H

#include "../LogErrors/NNLogErrors.h"

namespace Logic {
	template<class T>
	class TSingleton {
		public:
		TSingleton() {
			if(s_pInstance) {
				_ERROR(TO_STR(T)" is already created");
			}
			s_pInstance = this;
		}
		virtual ~TSingleton()=default;
		static T* Get() { return dynamic_cast<T*>(s_pInstance); }
		
		private:
		static TSingleton<T>* s_pInstance;
	};
	template<class T>
	TSingleton<T>* TSingleton<T>::s_pInstance = nullptr;

}

#endif //LAB6_TSINGLETON_H
