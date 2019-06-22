#include "pch.h"

std::string generate_random_string(int n) {
	constexpr char chset[] =
		"123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	auto r_char = [=, r_gen{ std::mt19937{ std::random_device{}() } }]() mutable {
		return chset[std::uniform_int_distribution<>{0, sizeof chset - 2}(r_gen)];
	};

	std::stringstream ss;

	for (int i = 0; i < n; i++) {
		ss << r_char();
	}

	return ss.str();
}

std::string generate_random_string(int n, std::string suffix) {
	std::string tmp = generate_random_string(n);

	std::stringstream ss;
	ss << tmp << suffix;
	tmp = ss.str();

	return tmp;
}