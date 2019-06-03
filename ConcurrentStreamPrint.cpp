#include <iostream>

#include "ConcurrentStreamPrint.hpp"

std::mutex ConcurrentStreamPrint::mtx_;

void ConcurrentStreamPrint::print_data(int thread_id, std::string class_name, std::string msg) {
	std::lock_guard<std::mutex> l_guard(ConcurrentStreamPrint::mtx_);
	std::cout << "[Thread " << thread_id << "] class " << class_name << ": " << msg << std::endl;
}

void ConcurrentStreamPrint::print_data(std::string class_name, std::string msg) {
	std::lock_guard<std::mutex> l_guard(ConcurrentStreamPrint::mtx_);
	std::cout << "Class " << class_name << ": " << msg << std::endl;
}