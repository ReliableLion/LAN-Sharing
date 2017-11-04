#ifndef BUFFER_H
#define BUFFER_H

#include <sstream>
#include <array>

#define BUFF_SIZE 1024	//1KB
typedef std::array<char, BUFF_SIZE> buffer_type;

const char* ArraytoString(buffer_type array_buf) {
	std::stringstream s;
	s.clear();
	for (auto i = std::begin(array_buf); i != std::end(array_buf); i++) {
		s << *i;
	}
	return s.str().c_str();
}

#endif // !BUFFER_H

