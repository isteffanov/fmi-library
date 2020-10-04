#include "ConsoleHandler.hpp"

const char* ConsoleHandler::BOOKS_DB = "books.bin";
const char* ConsoleHandler::USERS_DB = "users.bin";
const char* ConsoleHandler::ADMIN_USERNAME = "admin";
const char* ConsoleHandler::ADMIN_PASSWORD = "i<3c++";

void ConsoleHandler::begin()
{
	checkAdmin();
	checkBooks();
	std::cout << "Welcome to the FMI Library!\n" << std::endl;

	while (true) {
		getCommand();

		if (command == "open") open();
		else if (command == "close") close();
		else if (command == "help") help();

		else if (command == "login") login();
		else if (command == "logout") logout();
		else if (command == "books all") booksAll();
		else if (command == "books info") booksInfo();
		else if (command == "books find") booksFind();
		else if (command == "books sort") booksSort();
		else if (command == "books add") addBook();
		else if (command == "books remove") removeBook();
		else if (command == "users add") addUser();
		else if (command == "users remove") removeUser();
		else if (command == "exit") {
			if (stream.is_open()) stream.close();
			break;
		}
		else std::cout << "Unknown command! Enter help for more information :)" << std::endl;
	}
}

void ConsoleHandler::getCommand()
{
	std::cout << "Enter a command: ";
	std::cin >> command;

	if (command == "books" || command == "users") {
		String spec;
		std::cin >> spec;
		command += " ";
		command += spec;
	}
}

void ConsoleHandler::open()
{
	String filepath;
	std::cin >> filepath;

	stream.open(filepath.ptr(), std::ios::binary | std::ios::in | std::ios::out);
	if (stream.is_open()) std::cout << "Successfully opened " << filepath << "!" << std::endl;
	else std::cout << "File cannot be found! :(" << std::endl;
}

void ConsoleHandler::close()
{

	if (stream.is_open()) {
		stream.close();
		std::cout << "Successfully closed" << std::endl;
	}
	else std::cout << "No file to close!" << std::endl;
}

void ConsoleHandler::help()
{
	std::cout << "These are all the functions in the program: " << std::endl;

	std::cout << "login <username> <password>" << std::endl;
	std::cout << "logout" << std::endl;
	std::cout << "books all" << std::endl;
	std::cout << "books info <isbn>" << std::endl;
	std::cout << "books find <option> <option_string>" << std::endl;
	std::cout << "books sort <option> <asc | desc>" << std::endl;
	std::cout << "books add" << std::endl;
	std::cout << "books remove" << std::endl;
	std::cout << "users add" << std::endl;
	std::cout << "users remove <username>" << std::endl;
}

void ConsoleHandler::login()
{
	String name, pass;
	std::cout << "Enter username: ";
	name.getln();
	std::cout << "Enter password: ";
	pass.getln();
	bool admin = false;

	if (name == user.getUsername() && pass == user.getPassword()) {
		std::cout << "You are already logged in!" << std::endl;
		return;
	}
	stream.open(USERS_DB, std::ios::binary | std::ios::in | std::ios::out);	
	if (stream.is_open()) {
		while (!stream.eof()) {
			User cur_user;
			if (stream.is_open()) {
				try {
					if (stream.good()) stream >> cur_user;															//gets an user from the db
					else break;
					if (name == cur_user.getUsername() && pass == cur_user.getPassword()) {							//and compares its username and password with the given ones
						user = cur_user;																			//if they are the same a user object is created
						stream.close();
						std::cout << "Welcome, " << name << "!" << std::endl;
						return;
					}
				}
				catch (...) {
					break;
				}
			}
			else break;
		}
		stream.close();
		std::cout << "Wrong username or password!" << std::endl;
	}
	else std::cout << "Sorry, could not connect to the database!" << std::endl;

}

void ConsoleHandler::logout()
{
	user = User();
}

void ConsoleHandler::booksAll()
{
	if (!is_logged()) {
		std::cout << "You have to be logged to use this functionality! :(" << std::endl;
		return;
	}

	stream.open(BOOKS_DB, std::ios::binary | std::ios::in | std::ios::out);
	if (stream.is_open()) {
		Book book;
		while (!stream.eof()) {
			if (stream.good()) {
				try {
					if (stream.good()) stream >> book;										//get a book from the db
					std::cout << book << std::endl;											//and print it's info on the 
				}
				catch (...) {
					break;
				}
			}
		}
		stream.close();
	}
	else std::cout << "Could not open the database :(" << std::endl;
	
}

void ConsoleHandler::booksInfo()
{
	if (!is_logged()) {
		std::cout << "You have to be logged to use this functionality! :(" << std::endl;
		return;
	}

	uint64_t isbn;
	std::cin >> isbn;

	stream.open(BOOKS_DB, std::ios::binary | std::ios::in | std::ios::out);
	if (stream.is_open()) {
		Book book;
		while (!stream.eof()) {
			try {
				if(stream.good()) stream >> book;										//gets a book from the db and compares its isbn to the wanted one
				if (book.getISBN() == isbn) {
					std::cout << book;													//once found the book's info is printed on the console
					std::cin.get();
					break;
				}
			}
			catch (...) {
				std::cout << "Couldn't find a book with that ISBN :( Try again!" << std::endl;
				break;
			}
		}
		stream.close();
	}
}

void ConsoleHandler::booksFind()
{
	if (!is_logged()) {
		std::cout << "You have to be logged to use this functionality! :(" << std::endl;
		return;
	}

	String option, specification;

	std::cin >> option;
	specification.getln();

	List<Book> books = extractBooks();
	books.shrink_to_fit();

	if (option == "title") findByTitle(books, specification);
	else if (option == "author") findByAuthor(books, specification);
	else if (option == "tag") findByTag(books, specification);
	else std::cout << "You can search only titles, authors and tags!" << std::endl;
}

void ConsoleHandler::booksSort()
{
	if (!user.isadmin()) {
		std::cout << "You need admin rights to use that command!" << std::endl;
		return;
	}

	String option, type = "asc";
	std::cin >> option >> type;
	
	List<Book> books = extractBooks();
	books.shrink_to_fit();

	if (option == "title") sortByTitle(books, type);											//here the list of books is sorted in some way
	else if (option == "author") sortByAuthor(books, type);
	else if (option == "year") sortByYear(books, type);
	else if (option == "rating") sortByRating(books, type); 
	else std::cout << "Use one of 'titile', 'author' or 'tag' for the sorting!" << std::endl;

	stream.open(BOOKS_DB, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);	//then the whole db is truncated and filled again
	if (stream.is_open()) {
		for (int i = 0; i < books.size(); ++i)
			if(stream.good()) stream << books[i];

		stream.close();
	}

	std::cout << "Books sorted!" << std::endl;
}

void ConsoleHandler::addBook()
{
	if (!user.isadmin()) {
		std::cout << "You need admin rights to use that command!" << std::endl;
		return;
	}

	stream.open(BOOKS_DB, std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
	if (stream.is_open()) {
		Book book;

		std::cin >> book;																//receives a book from the console
		if(stream.good()) stream << book;																	//and enters it in the db
		
		stream.close();
	}
}

void ConsoleHandler::removeBook()
{
	if (!user.isadmin()) {
		std::cout << "You need admin rights to use that command!" << std::endl;
		return;
	}

	String title, author;
	std::cout << "Enter title: ";
	title.getln();
	std::cout << "Enter author: ";
	author.getln();

	List<Book> books = extractBooks();														//gets all the books in a list
	books.shrink_to_fit();

	for (int i = 0; i < books.size(); ++i)
		if (books[i].getTitle() == title && books[i].getAuthor() == author) books.remove(i); //removes the given book from the list

	stream.open(BOOKS_DB, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc); //truncates the whole db
	if (stream.is_open()) {
		for (int i = 0; i < books.size(); ++i)											  //and re-enters the books that are left 
			if(stream.good()) stream << books[i];

		stream.close();
	}
}

void ConsoleHandler::addUser()
{
	if (!user.isadmin()) {
		std::cout << "You need admin rights to use that command!" << std::endl;
		return;
	}

	stream.open(USERS_DB, std::ios::binary | std::ios::in | std::ios::out | std::ios::app);
	if (stream.is_open()) {
		User user;

		std::cin >> user;																		//creates non-admin user
		if(stream.good()) stream << user;														//enter the user in the db

		stream.close();
	}
}

void ConsoleHandler::removeUser()
{
	if (!user.isadmin()) {
		std::cout << "You need admin rights to use that command!" << std::endl;
		return;
	}

	String username;
	username.getln();

	if (username == "admin") {
		std::cout << "You cannot remove that user >:(" << std::endl;
		return;
	}
	if (username == user.getUsername()) {
		std::cout << "You cannot remove yourself!" << std::endl;
		return;
	}

	List<User> users = extractUsers();																	//get all the users in a list
	users.shrink_to_fit();

	for (int i = 0; i < users.size(); ++i)
		if (users[i].getUsername() == username) users.remove(i);										//removes the given user from the list

	stream.open(USERS_DB, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);			//truncates the db
	if (stream.is_open()) {
		for (int i = 0; i < users.size(); ++i)														//and re-enters all the users that are left
			stream << users[i];

		stream.close();
	}
}

bool ConsoleHandler::is_logged()
{
	return  ( !(user == User()) );
}

List<Book> ConsoleHandler::extractBooks()
{
	List<Book> books;
	stream.open(BOOKS_DB, std::ios::binary | std::ios::in | std::ios::out);
	if (stream.is_open()) {
		while (!stream.eof()) {
			Book book;
			if (stream.good()) {
				try {
					stream >> book;															//gets a book from the db
					books.add(book);														//adds it to the list
				}
				catch (...) {
					break;
				}
			}
		}
		stream.close();
	}

	return books;
}

List<User> ConsoleHandler::extractUsers()
{
	List<User> users;
	stream.open(USERS_DB, std::ios::binary | std::ios::in | std::ios::out);
	if (stream.is_open()) {
		while (!stream.eof()) {
			User user;
			if (stream.good()) {
				try {
					stream >> user;														//gets a user from the db
					users.add(user);													//adds it to the list
				}
				catch (...) {
					break;
				}
			}
		}
		stream.close();
	}

	return users;
}

void ConsoleHandler::sortByTitle(List<Book>& books, const String& sort_form)
{
	List<String> titles;
	for (int i = 0; i < books.size(); ++i)
		titles.add(books[i].getTitle());

	if (sort_form == "asc") titles.bubbleSortAsc();
	else if (sort_form == "desc") titles.bubbleSortDesc();
	
	for (int i = 0; i < books.size(); ++i) 
		for (int j = 0; j < titles.size(); ++j) 
			if (books[i].getTitle() == titles[j]) books.swap(i, j);			//orders the books the same way that their names are sorted in the list
}

void ConsoleHandler::sortByAuthor(List<Book>& books, const String& sort_form)
{
	List<String> authors;
	for (int i = 0; i < books.size(); ++i)
		authors.add(books[i].getAuthor());

	if (sort_form == "asc") authors.bubbleSortAsc();
	else if (sort_form == "desc") authors.bubbleSortDesc();

	for (int i = 0; i < books.size(); ++i)
		for (int j = 0; j < authors.size(); ++j)
			if (books[i].getAuthor() == authors[j]) books.swap(i, j);	//orders the books the same way that their authors are sorted in the list
}

void ConsoleHandler::sortByYear(List<Book>& books, const String& sort_form)
{
	List<int> years;
	for (int i = 0; i < books.size(); ++i)
		years.add(books[i].getYear());

	if (sort_form == "asc") years.bubbleSortAsc();
	else if (sort_form == "desc") years.bubbleSortDesc();

	for (int i = 0; i < books.size(); ++i)
		for (int j = 0; j < years.size(); ++j)
			if (books[i].getYear() == years[j]) books.swap(i, j);		//orders the books the same way that their release years are sorted in the list
}

void ConsoleHandler::sortByRating(List<Book>& books, const String& sort_form)
{
	double EPS = 0.01f;

	List<int> ratings;
	for (int i = 0; i < books.size(); ++i)
		ratings.add(books[i].getRating());

	if (sort_form == "asc") ratings.bubbleSortAsc();
	else if (sort_form == "desc") ratings.bubbleSortDesc();

	for (int i = 0; i < books.size(); ++i)
		for (int j = 0; j < ratings.size(); ++j)
			if (abs(books[i].getRating() - ratings[j]) < EPS) books.swap(i, j);		//orders the books the same way that their ratings are sorted in the list
}

void ConsoleHandler::findByTitle(List<Book>& books, const String& title)
{
	bool found = false;;
	for (int i = 0; i < books.size(); ++i) {
		if (books[i].getTitle() == title) {
			found = true;
			books[i].print();
			std::cout << std::endl;;
		}
	}
	if (!found) std::cout << "Sorry, couldn't find anything :(" << std::endl;
}

void ConsoleHandler::findByAuthor(List<Book>& books, const String& author)
{
	bool found = false;
	for (int i = 0; i < books.size(); ++i) {
		if (books[i].getAuthor() == author) {
			found = true;
			books[i].print();
			std::cout << std::endl;;
		}
	}
	if (!found) std::cout << "Sorry, couldn't find anything :(" << std::endl;
}

void ConsoleHandler::findByTag(List<Book>& books, const String& tag)
{
	bool found = false;
	for (int i = 0; i < books.size(); ++i) {
		for (int j = 0; j < books[i].getKeywords().size(); ++j) {
			if (books[i].getKeywords()[j] == tag) {
				found = true;
				books[i].print();
				std::cout << std::endl;;
			}
		}
	}
	if (!found) std::cout << "Sorry, couldn't find anything :(" << std::endl;		
}

void ConsoleHandler::checkAdmin()
{
	stream.open(USERS_DB, std::ios::binary | std::ios::in | std::ios::out);
	if (stream.is_open()) {
		stream.close();
		return;
	}

	//else register the admin users
	unsigned name_len = strlen(ADMIN_USERNAME);
	char* admin_name = new char[name_len + 1];
	strcpy(admin_name, ADMIN_USERNAME);
	unsigned pass_len = strlen(ADMIN_PASSWORD);
	char* admin_pass = new char[pass_len + 1];
	strcpy(admin_pass, ADMIN_PASSWORD);
	int admin_rights = 1; //true

	std::ofstream s(USERS_DB, std::ios::binary);
	if (s.is_open()) {
		s.write((const char*)&name_len, sizeof(unsigned));
		s.write(admin_name, name_len);
		s.write((const char*)&pass_len, sizeof(unsigned));
		s.write(admin_pass, pass_len);
		s.write((const char*)&admin_rights, sizeof(int));
		s.close();
	}
	delete[] admin_name;
	delete[] admin_pass;
}

void ConsoleHandler::checkBooks()
{
	stream.open(BOOKS_DB, std::ios::binary | std::ios::in | std::ios::out);
	if (stream.is_open()) {
		stream.close();
		return;
	}

	//else create the books db file
	std::ofstream s(BOOKS_DB, std::ios::binary);
	if (s.is_open()) s.close();
}