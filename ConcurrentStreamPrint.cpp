#include "pch.h"

std::mutex ConcurrentStreamPrint::mtx_;

inline std::tm localtime_xp(std::time_t timer)
{
    std::tm bt {};
#if defined(__unix__)
    localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
    localtime_s(&bt, &timer);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    bt = *std::localtime(&timer);
#endif
    return bt;
}

// default = "YYYY-MM-DD HH:MM:SS"
inline std::string time_stamp(time_t time_stamp_t, const std::string& fmt = "%F %T")
{
    auto bt = localtime_xp(time_stamp_t);
    char buf[64];
    return {buf, std::strftime(buf, sizeof(buf), fmt.c_str(), &bt)};
}
void ConcurrentStreamPrint::print_data(int thread_id, std::string class_name, std::string msg) {
	auto end = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);
	std::lock_guard<std::mutex> l_guard(ConcurrentStreamPrint::mtx_);
	std::cout << time_stamp(end_time) << " [Thread " << thread_id << "] class " << class_name << ": " << msg << std::endl;
}

void ConcurrentStreamPrint::print_data(std::string class_name, std::string msg) {
	std::lock_guard<std::mutex> l_guard(ConcurrentStreamPrint::mtx_);
	std::cout << "Class " << class_name << ": " << msg << std::endl;
}