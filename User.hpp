#pragma once
#include <string>
#include <utility>

class user {
	
public:

	user() = default;
	user(std::string username, std::string client_address): username(std::move(username)), client_address(std::move(client_address)) {}

	std::string username;
	std::string client_address;

	// overload < operator to use a Node object as a key in a std::map
    // It returns true if the current object appears before the specified object
    bool operator<(const user &ob) const
    {
        return username < ob.username || (username == ob.username && client_address < ob.client_address);
    }
};
