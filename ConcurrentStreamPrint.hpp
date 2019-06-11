#pragma once

#include <string>
#include <mutex>

class ConcurrentStreamPrint {
public: 
	ConcurrentStreamPrint() {};
	
	static void print_data(int thread_id, std::string class_name, std::string msg);

	static void print_data(std::string class_name, std::string msg);

private:
	static std::mutex mtx_;
};