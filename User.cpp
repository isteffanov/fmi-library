#include "User.hpp"

User::User(String username, String password, bool admin)
	:m_username(username), m_password(password), m_admin(admin) {}

const String& User::getUsername() const
{
	return m_username;
}

const String& User::getPassword() const
{
	return m_password;
}

bool User::isadmin()
{
	return m_admin;
}

bool User::operator==(const User& other) const
{
	return (m_username == other.m_username && m_password == other.m_password && m_admin == other.m_admin);
}

std::istream& operator>>(std::istream& stream, User& user)
{
	String username, password;
	std::cout << "Enter username: ";
	username.getln();
	std::cout << "Enter password: ";
	password.getln();

	user = User(username, password); // creates non admin user

	return stream;
}

std::fstream& operator<<(std::fstream& stream, User& user)
{
	if (stream.good()) {
		unsigned username_len = user.m_username.len();
		char* c_username = user.m_username.ptr();
		unsigned password_len = user.m_password.len();
		char* c_password = user.m_password.ptr();
		int admin = (user.m_admin ? 1 : 0);

		stream.write((const char*)&username_len, sizeof(unsigned));
		stream.write(c_username, username_len);
		stream.write((const char*)&password_len, sizeof(unsigned));
		stream.write(c_password, password_len);
		stream.write((const char*)&admin, sizeof(admin));
	}

	return stream;
}

std::fstream& operator>>(std::fstream& stream, User& user)
{
	unsigned user_len;
	unsigned pass_len;
	int int_bool_admin;

	stream.read((char*)&user_len, sizeof(unsigned));
	char* c_name = new char[user_len + 1];
	stream.read(c_name, user_len);
	c_name[user_len] = '\0';

	stream.read((char*)&pass_len, sizeof(unsigned));
	char* c_pass = new char[pass_len + 1];
	stream.read(c_pass, pass_len);
	c_pass[pass_len] = '\0';

	stream.read((char*)&int_bool_admin, sizeof(int));
	bool admin = (int_bool_admin == 1 ? true : false);

	user = User(c_name, c_pass, admin);

	delete[] c_name;
	delete[] c_pass;

	return stream;
}
