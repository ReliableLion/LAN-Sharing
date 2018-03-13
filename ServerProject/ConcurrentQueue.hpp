#pragma once
#include "Constants.hpp"
#include <queue>
#include <mutex>

template <typename T>
class ConcurrentQueue {
	std::queue<T> c_queue;
	int element_n;
	const int max_element = QUEUE_MAX_ELEMENTS;
	std::mutex mtx;
public:
	ConcurrentQueue() { element_n = 0; }
	~ConcurrentQueue() {}

	/**
	 * \brief isert an element into the the concurrent queue
	 * \param element
	 * \return return TRUE if the data is pushed correctly, FALSE othrewise
	 */
	bool insertElement(T element) {
		std::lock_guard<std::mutex> queue_lock(mtx);
		
		// if the queue exceed the maximum capability return false
		if (element_n == max_element) { return false; }
		c_queue.push(element);
		element_n++;
		return true;
	}

	
	/**
	 * \brief pop one element at time from the queue
	 * \param element: used to insert the value of the popped data 
	 * \return return TRUE if the element is popped correctly, FALSE if cannot performed a pop on  the queue
	 */
	bool popElement(T& element) {
		std::lock_guard<std::mutex> queue_lock(mtx);
		
		// if the queue go under 0 element return false
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
