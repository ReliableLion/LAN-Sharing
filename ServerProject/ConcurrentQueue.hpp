#pragma once

#include <queue>
#include <mutex>

#include "Constants.hpp"

template <typename T>
class ConcurrentQueue {
	std::queue<T> c_queue_;
	int element_n_;
	const int max_element_ = QUEUE_MAX_ELEMENTS;
	std::mutex mtx_;
public:
	ConcurrentQueue() { element_n_ = 0; }
	~ConcurrentQueue() {}

	/**
	 * \brief isert an element into the the concurrent queue
	 * \param element
	 * \return return TRUE if the data is pushed correctly, FALSE othrewise
	 */
	bool insertElement(T element) {
		std::lock_guard<std::mutex> queue_lock(mtx_);
		
		// if the queue exceed the maximum capability return false
		if (element_n_ == max_element_) { return false; }
		c_queue_.push(element);
		element_n_++;
		return true;
	}

	/**
	 * \brief pop one element at time from the queue
	 * \param element: used to insert the value of the popped data 
	 * \return return TRUE if the element is popped correctly, FALSE if cannot performed a pop on  the queue
	 */
	bool popElement(T& element) {
		std::lock_guard<std::mutex> queue_lock(mtx_);
		
		// if the queue go under 0 element return false
		if (element_n_ == 0) { return false; }
		element = c_queue_.front();
		c_queue_.pop();
		element_n_--;
		return true;
	}

	bool isEmpty() { 
		std::lock_guard<std::mutex> queue_lock(mtx_);
		return c_queue_.empty();
	}
};
