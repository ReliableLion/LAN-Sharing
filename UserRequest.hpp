#pragma once
#include "User.hpp"

class user_request {
	
public:

	user_request() = default;
	user_request(const user destination_user, const std::string file_name): destination_user(destination_user), file_name(file_name) {}

	user destination_user;
	std::string file_name;

		// overload < operator to use a Node object as a key in a std::map
    // It returns true if the current object appears before the specified object
    bool operator<(const user_request &ob) const
    {
        return destination_user.username < ob.destination_user.username || (destination_user.username == ob.destination_user.username && destination_user.client_address < ob.destination_user.client_address);
    }
};
