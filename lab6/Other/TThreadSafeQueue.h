//
// Created by choleraplague on 06.12.22.
//

#ifndef UNTITLED1_TTHREADSAFEQUEUE_H
#define UNTITLED1_TTHREADSAFEQUEUE_H

#include <shared_mutex>
#include <queue>

template <typename T>
class TThreadSafeQueue {
	public:
	TThreadSafeQueue()=default;
	~TThreadSafeQueue()=default;
	
	template<typename Type>
	void Push(Type&& param);
	void Pop();
	T& Front();
	bool Empty();
	
	protected:
	std::queue<T> m_qQueue;
	std::shared_mutex m_xMutex;
};

template<typename T>
template<typename Type>
void TThreadSafeQueue<T>::Push(Type&& param) {
	std::lock_guard<std::shared_mutex> lock(m_xMutex);
	m_qQueue.push(std::forward<Type>(param));
}

template<typename T>
void TThreadSafeQueue<T>::Pop() {
	std::lock_guard<std::shared_mutex> lock(m_xMutex);
	m_qQueue.pop();
}

template<typename T>
T& TThreadSafeQueue<T>::Front() {
	std::shared_lock<std::shared_mutex> lock(m_xMutex);
	return m_qQueue.front();
}

template<typename T>
bool TThreadSafeQueue<T>::Empty() {
	std::shared_lock<std::shared_mutex> lock(m_xMutex);
	return m_qQueue.empty();
}

#endif //UNTITLED1_TTHREADSAFEQUEUE_H
