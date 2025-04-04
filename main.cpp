#include "main.hpp"
#include "student.hpp"
#include "admin.hpp"

constexpr std::string_view ASCII_ART = R"(
          _____        _____                     _____
         /\    \      /\    \                   /\    \
        /::\____\    /::\____\                 /::\    \
       /:::/    /   /::::|   |                /::::\    \
      /:::/    /   /:::::|   |               /::::::\    \
     /:::/    /   /::::::|   |              /:::/\:::\    \
    /:::/    /   /:::/|::|   |             /:::/__\:::\    \
   /:::/    /   /:::/ |::|   |             \:::\   \:::\    \
  /:::/    /   /:::/  |::|___|______     ___\:::\   \:::\    \
 /:::/    /   /:::/   |::::::::\    \   /\   \:::\   \:::\    \
/:::/____/   /:::/    |:::::::::\____\ /::\   \:::\   \:::\____\
\:::\    \   \::/    / ~~~~~/:::/    / \:::\   \:::\   \::/    /
 \:::\    \   \/____/      /:::/    /   \:::\   \:::\   \/____/
  \:::\    \              /:::/    /     \:::\   \:::\    \
   \:::\    \            /:::/    /       \:::\   \:::\____\
    \:::\    \          /:::/    /         \:::\  /:::/    /
     \:::\    \        /:::/    /           \:::\/:::/    /
      \:::\    \      /:::/    /             \::::::/    /
       \:::\____\    /:::/    /               \::::/    /
        \::/    /    \::/    /                 \::/    /
         \/____/      \/____/                   \/____/
)";


const std::string Book::toString() const {
    return "Title: " + title + ", Author: " + author + ", Quantity: " + std::to_string(quantity);
}

const bool Book::operator==(const Book& other) const {
    return title == other.title and author == other.author; // Not checking the quantity
}
const bool Book::operator!=(const Book& other) const {
    return title != other.title or author != other.author; // Not checking the quantity
}

void showTable(std::vector<std::array<std::string, 3>> table, const std::array<std::string, 3>& headers) {
    // Find the max length of each column
    std::array<size_t, 3> max_lengths = {headers[0].size(), headers[1].size(), headers[2].size()};
    for (const auto& row : table) {
        for (size_t i = 0; i < row.size(); ++i) {
            max_lengths[i] = std::max(max_lengths[i], row[i].length());
        }
    }

    // Print the headers
    std::cout << "  | ";
    for (size_t i = 0; i < 3; ++i) {
        size_t padding = std::max(max_lengths[i], headers[i].size()) - headers[i].size() + 2;
        std::cout << headers[i] << std::string(padding, ' ') << " | ";
    }
    std::cout << std::endl;

    // Print the separator line
    std::cout << "  +-";
    for (size_t i = 0; i < 2; ++i) {
        size_t padding = max_lengths[i] + 2;
        std::cout << std::string(padding, '-') << "-+-";
    }
    size_t padding = max_lengths[2] + 2;
    std::cout << std::string(padding, '-') << "-+ ";
    std::cout << std::endl;

    // Print the actual table
    for (auto row : table) {
        std::cout << "  | ";
        for (size_t i = 0; i < 3; ++i) {
            size_t padding = std::max(max_lengths[i], row[i].size()) - row[i].size() + 2;
            std::cout << row[i] << std::string(padding, ' ') << " | ";
        }
        std::cout << std::endl;
    }
}

void showBooks() {
    std::ifstream file(BOOKS_PATH);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    std::vector<std::array<std::string, 3>> table; // 3 is true enough for books.csv and issued_books.csv

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::string title, author;
        int quantity;
        std::getline(line_stream, title, '#');
        std::getline(line_stream, author, '#');
        line_stream >> quantity;

        table.push_back({title, author, std::to_string(quantity)});
    }

    showTable(table, {"Title", "Author", "Quantity"});
}

Book getBook(const std::string& title, const std::string& author) {
    std::ifstream file(BOOKS_PATH);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return Book{};
    }
    Book book;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::getline(line_stream, book.title, '#');
        std::getline(line_stream, book.author, '#');
        line_stream >> book.quantity;

        if (book.title == title && book.author == author) {
            return book;
        }
    }
    return Book{}; // Return an empty book if not found
}

void searchBook() {
    std::string search_title, search_author;
    std::cin.ignore();
    std::cout << "Enter book title: ";
    std::getline(std::cin, search_title);
    std::cout << "Enter book author: ";
    std::getline(std::cin, search_author);

    Book book = getBook(search_title, search_author);
    if (book.title.empty()) {
        std::cout << "Book not found." << std::endl;
        return;
    }

    std::cout << "Book found: " << book.toString() << std::endl;
}

void viewIssuedBooks(const std::string& roll_number) {
    std::ifstream file(ISSUED_BOOKS_PATH);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {

    }
    file.close();
    if (line.empty()) {
        std::cout << "No issued books found for " << roll_number << "." << std::endl;
    }
}

const std::string hashPassword(const std::string& password) {
    std::string hashedPassword = password; // Placeholder for hashing logic
    return hashedPassword;
}

void signIn() {
    while (true) {
        std::cout << "Are you signing in as an admin? (y/n): ";
        char choice; std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            signInAsAdmin();
            return;
        } else if (choice == 'n' || choice == 'N') {
            signInAsStudent();
            return;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

bool usernameExists(const std::string& file_path, const std::string& search_username) {
    std::ifstream file(file_path);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return false;
    }

    std::string line; // Using stringstreams instead of objects so it can work for both students and teachers
    while (std::getline(file, line)) {
        std::istringstream iss(line);

        std::string username, password;
        std::getline(iss, username, '#');
        std::getline(iss, password, '#');

        if (username == search_username) {
            return true;
        }
    }

    file.close();
    return false;
}

bool validateAdmin(const std::string& search_username, const std::string& search_password) {
    std::ifstream file(ADMIN_PATH);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return false;
    }

    std::string hashed_password = hashPassword(search_password);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);

        std::string username, password;
        std::getline(iss, username, '#');
        std::getline(iss, password, '#');

        if (username == search_username && password == hashed_password) {
            return true;
        }
    }

    return false;
}

void signInAsAdmin() {
    std::string search_username, search_password;
    std::cout << "Enter username: "; std::cin >> search_username;
    std::cout << "Enter password: "; std::cin >> search_password;

    if (!validateAdmin(search_username, search_password)) {
        std::cout << "Invalid username or password." << std::endl;
        return;
    }
    
    handleAdminSession(search_username);
}

void signInAsStudent() {
    std::string search_roll_number, search_password;
    int issued_books_count = -1;
    std::cout << "Enter roll number: "; std::cin >> search_roll_number;
    std::cout << "Enter password: "; std::cin >> search_password;

    std::ifstream file(USER_PATH);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    std::string hashed_password = hashPassword(search_password);
    bool found = false;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);

        std::string username, password;
        std::getline(iss, username, '#');
        std::getline(iss, password, '#');
        iss >> issued_books_count;

        if (username == search_roll_number && password == hashed_password) {
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "Invalid username or password." << std::endl;
    }

    Student student{search_roll_number, search_password, issued_books_count};

    handleStudentSession(student);
}

void signUpAsStudent() {
    std::string roll_number, password;
    std::cout << "Enter roll number: "; std::cin >> roll_number;
    std::cout << "Enter password: "; std::cin >> password;

    std::ofstream file(USER_PATH, std::ios::app);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }
    if (usernameExists(USER_PATH, roll_number)) {
        std::cout << "Roll number already exists. Please try again." << std::endl;
        return;
    }
    file << roll_number << "#" << hashPassword(password) << "#" << "0" << '\n'; // Assuming 0 issued books at the start
    file.close();

    std::cout << "Student signed up successfully." << std::endl;
    std::cout << "Please sign in to continue." << std::endl;
}

void signUpAsAdmin() {
    std::string username, password;
    std::cout << "Enter username: "; std::cin >> username;
    std::cout << "Enter password: "; std::cin >> password;

    std::ofstream file(ADMIN_PATH, std::ios::app);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }
    if (usernameExists(ADMIN_PATH, username)) {
        std::cout << "Username already exists. Please try again." << std::endl;
        return;
    }
    file << username << "#" << hashPassword(password) << '\n';
    file.close();

    std::cout << "Admin signed up successfully." << std::endl;
    std::cout << "Please sign in to continue." << std::endl;
}

void signUp() {
    std::cout << "Are you signing up as an admin? (y/n): ";
    char choice; std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        signUpAsAdmin();
    } else if (choice == 'n' || choice == 'N') {
        signUpAsStudent();
    } else {
        std::cout << "Invalid choice. Please try again." << std::endl;
    }
}


int main(void) {
    std::cout << ASCII_ART << std::endl;
    std::cout << "Library Management System" << std::endl;
    std::cout << "Main Menu" << std::endl;
    while (true) {
        std::cout << R"(Please choose an option:
            1. Sign In
            2. Sign Up
            9. Exit
        )" << std::endl;
        int choice; std::cin >> choice;

        switch (choice) {
            case 1: {
                signIn();
                break;
            }
            case 2: {
                signUp();
                break;
            }
            case 3: {
                Student tempStudent{"1", "1", 0};
                handleStudentSession(tempStudent);
                break;
            }
            case 4: {
                handleAdminSession("Developer");
                break;
            }
            case 9: {
                return 0;
            }
            default: {
                std::cout << "Invalid choice. Please try again." << std::endl;
            }
        }
    }
}