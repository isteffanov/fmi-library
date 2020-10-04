#pragma once
#include "String.hpp"

class User
{
	String m_username;
	String m_password;
	bool m_admin;

public:
	User(String username = "Default User", String password = "Default Pass", bool admin = false);

	const String& getUsername() const;
	const String& getPassword() const;

	bool isadmin();
	bool operator==(const User& other) const;

	friend std::istream& operator>>(std::istream& stream, User& user);
	friend std::fstream& operator<<(std::fstream& stream, User& user);
	friend std::fstream& operator>>(std::fstream& stream, User& user);
};

