#ifndef ADMIN_HPP
#define ADMIN_HPP

#include <string>
#include "admin.hpp"

// Function Declarations
void addBook();
void removeBook();
void viewIssuedBooksForInputStudent();
void handleAdminSession(const std::string& username);

#endif // ADMIN_HPP