#pragma once
#include <iomanip>
#include "String.hpp"
#include "List.hpp"

class Book
{
	String m_title;
	String m_author;
	String m_genre;
	String m_description;
	int m_releaseyear;
	List<String> m_keywords;
	double m_rating;
	uint64_t m_isbn;

public:
	Book(String title = "", String author = "", String genre = "", String description = "",
		int releaseyear = 0, List<String> keywords = List<String>(), double rating = 0.0f, uint64_t isbn = 0);

	const String& getTitle() const;
	const String& getAuthor() const;
	const List<String>& getKeywords() const;
	int getYear() const;
	double getRating() const;
	uint64_t getISBN() const;

	friend std::fstream& operator<<(std::fstream& stream, Book& book);
	friend std::fstream& operator>>(std::fstream& stream, Book& book);
	friend std::ostream& operator<<(std::ostream& stream, Book& book);
	friend std::istream& operator>> (std::istream& stream, Book& book);

	void print() const;
};

