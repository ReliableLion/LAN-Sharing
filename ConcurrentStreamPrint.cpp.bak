#include <iostream>
#include <chrono>
#include <ctime>

#include "ConcurrentStreamPrint.hpp"

std::mutex ConcurrentStreamPrint::mtx_;

void ConcurrentStreamPrint::print_data(int thread_id, std::string class_name, std::string msg) {
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::lock_guard<std::mutex> l_guard(ConcurrentStreamPrint::mtx_);
	std::cout << std::ctime(&end_time) << " [Thread " << thread_id << "] class " << class_name << ": " << msg << std::endl;
}

void ConcurrentStreamPrint::print_data(std::string class_name, std::string msg) {
	std::lock_guard<std::mutex> l_guard(ConcurrentStreamPrint::mtx_);
	std::cout << "Class " << class_name << ": " << msg << std::endl;
}