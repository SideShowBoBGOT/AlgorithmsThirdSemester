//
// Created by choleraplague on 18.11.22.
//

#ifndef UNTITLED1_TSINGLETON_H
#define UNTITLED1_TSINGLETON_H

template<class T>
class TSingleton {
	public:
	TSingleton() {
		s_pInstance = this;
	}
	virtual ~TSingleton()=default;
	static T* Get() { return dynamic_cast<T*>(s_pInstance); }
	
	private:
	static TSingleton<T>* s_pInstance;
};
template<class T>
TSingleton<T>* TSingleton<T>::s_pInstance = nullptr;


#endif //UNTITLED1_TSINGLETON_H
