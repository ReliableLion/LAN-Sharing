#pragma once
#include "Constant.hpp"
#include <queue>
#include <mutex>

template <typename T>
class ConcurrentQueue {
private:
	std::queue<T> c_queue;
	int element_n;
	const int max_element = QUEUE_MAX_ELEMENTS;
	std::mutex mtx;
public:
	ConcurrentQueue() { element_n = 0; }
	~ConcurrentQueue() {}

	bool insertElement(T element) {
		std::lock_guard<std::mutex> queue_lock(mtx);
		if (element_n == max_element) { return false; }
		c_queue.push(element);
		element_n++;
		return true;
	}

	bool popElement(T& element) {
		std::lock_guard<std::mutex> queue_lock(mtx);
		if (element_n == 0) { return false; }
		element = c_queue.front();
		c_queue.pop();
		element_n--;
		return true;
	}

	bool isEmpty() { 
		std::lock_guard<std::mutex> queue_lock(mtx);
		return c_queue.empty();
	}
};
