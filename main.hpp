#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cstdio> // for perror

#define USER_PATH "data\\students.csv"
#define TEMP_USER_PATH "data\\temp_students.csv"
#define ADMIN_PATH "data\\admins.csv"
#define BOOKS_PATH "data\\books.csv"
#define TEMP_BOOKS_PATH "data\\temp_books.csv"
#define ISSUED_BOOKS_PATH "data\\issued_books.csv"
#define TEMP_ISSUED_BOOKS_PATH "data\\temp_issued_books.csv"

struct Book {
    std::string title;
    std::string author;
    int quantity;

    const std::string toString() const;
    const bool operator==(const Book& other) const;
    const bool operator!=(const Book& other) const;
};

struct Student {
    std::string roll_number;
    std::string password;
    int issued_books_count;
};

void showBooks();
void showTable(std::vector<std::array<std::string, 3>> table, const std::array<std::string, 3>& headers);
Book getBook(const std::string& title, const std::string& author);
void searchBook();
void viewIssuedBooks(const std::string& roll_number);
const std::string hashPassword(const std::string& password);
void signIn();
void signInAsAdmin();
void signInAsStudent();
void signUpAsStudent();

#endif // MAIN_HPP