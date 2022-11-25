//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TSINGLETON_H
#define UNTITLED1_TSINGLETON_H

template<class T>
class TSingleton {
	public:
	virtual ~TSingleton()=default;
	static T* Get() {
		if(!s_pInstance) {
			s_pInstance = new T();
		}
		return dynamic_cast<T*>(s_pInstance);
	}
	
	protected:
	TSingleton()=default;
	static T* s_pInstance;
};
template<class T>
T* TSingleton<T>::s_pInstance = nullptr;

#endif //UNTITLED1_TSINGLETON_H
