#include "Book.hpp"

Book::Book(String title, String author, String genre, String description, int releaseyear, List<String> keywords, double rating, uint64_t isbn)
	:m_title(title), m_author(author), m_genre(genre) ,m_description(description), 
		m_releaseyear(releaseyear), m_keywords(keywords), m_rating(rating), m_isbn(isbn) {}

const String& Book::getTitle() const
{
	return m_title;
}

const String& Book::getAuthor() const
{
	return m_author;
}

const List<String>& Book::getKeywords() const
{
	return m_keywords;
}

int Book::getYear() const
{
	return m_releaseyear;
}

double Book::getRating() const
{
	return m_rating;
}

uint64_t Book::getISBN() const
{
	return m_isbn;
}

std::fstream& operator<<(std::fstream& stream, Book& book)
{
	if (stream.good()) {
		unsigned title_len = book.m_title.len();
		char* title = book.m_title.ptr();
		unsigned author_len = book.m_author.len();
		char* author = book.m_author.ptr();
		unsigned genre_len = book.m_genre.len();
		char* genre = book.m_genre.ptr();
		unsigned description_len = book.m_description.len();
		char* description = book.m_description.ptr();

		stream.write((const char*)&title_len, sizeof(unsigned));
		stream.write(title, title_len);
		stream.write((const char*)&author_len, sizeof(unsigned));
		stream.write(author, author_len); 
		stream.write((const char*)&genre_len, sizeof(unsigned)); 
		stream.write(genre, genre_len); 
		stream.write((const char*)&description_len, sizeof(unsigned)); 
		stream.write(description, description_len); 

		unsigned keywords_len = book.m_keywords.size();
		stream.write((const char*)&keywords_len, sizeof(unsigned));
		for (int i = 0; i < keywords_len; ++i) {
			unsigned len = book.m_keywords[i].len();
			char* keyword = book.m_keywords[i].ptr();
			stream.write((const char*)&len, sizeof(unsigned));
			stream.write(keyword, len);
		}
		stream.write((const char*)&book.m_releaseyear, sizeof(int));
		stream.write((const char*)&book.m_rating, sizeof(double));
		stream.write((const char*)&book.m_isbn, sizeof(uint64_t));
	}

	return stream;
}

std::fstream& operator>>(std::fstream& stream, Book& book)
{
	char* title, * author, * genre, * descr;
	int releaseyear;
	uint64_t isbn;
	double rating;
	unsigned title_len, author_len, genre_len, descr_len, keywords_len;
	
	if (stream.good()) {
		stream.read((char*)&title_len, sizeof(unsigned));
		title = new char[title_len + 1];
		stream.read(title, title_len);
		title[title_len] = '\0';

		stream.read((char*)&author_len, sizeof(unsigned));
		author = new char[author_len + 1];
		stream.read(author, author_len);
		author[author_len] = '\0';

		stream.read((char*)&genre_len, sizeof(unsigned));
		genre = new char[genre_len + 1];
		stream.read(genre, genre_len);
		genre[genre_len] = '\0';

		stream.read((char*)&descr_len, sizeof(unsigned));
		descr = new char[descr_len + 1];
		stream.read(descr, descr_len);
		descr[descr_len] = '\0';

		List<String> keywords;
		stream.read((char*)&keywords_len, sizeof(unsigned));
		for (int i = 0; i < keywords_len; ++i) {
			unsigned len;
			char* word;
			stream.read((char*)&len, sizeof(unsigned));
			word = new char[len + 1];
			stream.read(word, len);
			word[len] = '\0';

			keywords.add(String(word));
			delete[] word;
		}

		stream.read((char*)&releaseyear, sizeof(int));
		stream.read((char*)&rating, sizeof(double));
		stream.read((char*)&isbn, sizeof(uint64_t));


		book = Book(String(title), String(author), String(genre), String(descr), releaseyear, keywords, rating, isbn);

		delete[] title;
		delete[] author;
		delete[] genre;
		delete[] descr;
	}

	return stream;
}

std::ostream& operator<<(std::ostream& stream, Book& book)
{
	stream << "Title: " << book.m_title << std::endl;
	stream << "Author: " << book.m_author << std::endl;
	stream << "Genre: " << book.m_genre << std::endl;
	stream << "ISBN: " << book.m_isbn << std::endl;

	return stream;
}

std::istream& operator>>(std::istream& stream, Book& book)
{
	String title, author, genre, description, keywords;
	std::cout << "Enter title: ";
	title.getln(stream);
	std::cout << "Enter author: ";
	author.getln(stream);
	std::cout << "Enter genre: ";
	genre.getln(stream);
	std::cout << "Enter short description: ";
	description.getln(stream);
	std::cout << "Enter a few keywords: ";
	keywords.getln(stream);
	List<String> keywords_list = keywords.strip();
	std::cout << "Enter year of release year, rating and ISBN number: ";
	int releaseyear; 
	uint64_t isbn;
	double rating;
	std::cin >> releaseyear >> rating >> isbn;

	book = Book(title, author, genre, description, releaseyear, keywords_list, rating, isbn);

	return stream;
}

void Book::print() const
{
	std::cout << "Title: " << m_title << std::endl;
	std::cout << "Author: " << m_author << std::endl;
	std::cout << "Genre: " << m_genre << std::endl;
	std::cout << "Description: " << m_description << std::endl;
	std::cout << "Keywords: ";
	m_keywords.print();
	std::cout << "Year of release: " << m_releaseyear << std::endl;
	std::cout << "Rating: " << std::fixed << std::setprecision(2) << m_rating << std::endl;
	std::cout << "ISBN: " << m_isbn << std::endl;
}