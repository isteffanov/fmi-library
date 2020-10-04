#pragma once
#include "String.hpp"
#include "Book.hpp"
#include "User.hpp"
#include <iostream>
#include <fstream>

class ConsoleHandler
{
	const static char* BOOKS_DB;
	const static char* USERS_DB;
	const static char* ADMIN_USERNAME;
	const static char* ADMIN_PASSWORD;

	User user;
	String command;
	String filepath;
	std::fstream stream;

	void getCommand();
	void open();
	void close();
	void help();

	void login();
	void logout();
	void booksAll();
	void booksInfo();
	void booksFind();
	void booksSort();
	void addBook();
	void removeBook();
	void addUser();
	void removeUser();

	bool is_logged();
	List<Book> extractBooks(); //returns list with all of the books in the db
	List<User> extractUsers(); //returns list with all of the users in the db

	void sortByTitle(List<Book>& books, const String& sort_form);
	void sortByAuthor(List<Book>& books, const String& sort_form);
	void sortByYear(List<Book>& books, const String& sort_form);
	void sortByRating(List<Book>& books, const String& sort_form);
	void findByTitle(List<Book>& books, const String& title);
	void findByAuthor(List<Book>& books, const String& author);
	void findByTag(List<Book>& books, const String& tag);

	void checkAdmin(); //creates an admin user if there isnt one yet
	void checkBooks(); //creates the books db file if there isnt one yet
public:
	void begin();
};

