#include "main.hpp"


void addNewBook(Book book) {
    std::ofstream file(BOOKS_PATH, std::ios::app);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }
    file << book.title << "#" << book.author << "#" << book.quantity << "\n";
    file.close();
}

void addBook() {
    Book book;
    std::cin.ignore();
    std::cout << "Enter book title: ";
    std::getline(std::cin, book.title);
    std::cout << "Enter book author: ";
    std::getline(std::cin, book.author);
    std::cout << "Enter book quantity: ";
    std::cin >> book.quantity;


    if (getBook(book.title, book.author) != book) {
        addNewBook(book);
        std::cout << "Book added successfully." << std::endl;
        return;
    }


    std::ifstream books_file(BOOKS_PATH);
    if (!books_file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }
    std::ofstream temp_books_file(TEMP_BOOKS_PATH);
    if (!temp_books_file) {
        std::cout << "Error creating temporary file." << std::endl;
        books_file.close();
        return;
    }
    Book scan_book;
    std::string scan_line;
    while (std::getline(books_file, scan_line)) {
        std::istringstream line_stream(scan_line);
        std::getline(line_stream, scan_book.title, '#');
        std::getline(line_stream, scan_book.author, '#');
        line_stream >> scan_book.quantity;

        if (scan_book == book) {
            scan_book.quantity += book.quantity;
        }
        temp_books_file << scan_book.title << "#" << scan_book.author << "#" << scan_book.quantity << "\n";
    }
    books_file.close();
    temp_books_file.close();
    std::remove(BOOKS_PATH); // On my system for some reason std::rename doesn't work (doesn't overwrite) if I don't remove the file first
    if (std::rename(TEMP_BOOKS_PATH, BOOKS_PATH) != 0) {
        std::cout << "Error updating books file." << std::endl;
        return;
    }
    std::cout << "Book quantity updated successfully." << std::endl;
}

void removeBook() {
    std::string title, author;
    std::cin.ignore();
    std::cout << "Enter book title to remove: ";
    std::getline(std::cin, title);
    std::cout << "Enter book author to remove: ";
    std::getline(std::cin, author);


    std::ifstream file(BOOKS_PATH);
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return;
    }
    std::ofstream temp_file(TEMP_BOOKS_PATH);
    if (!temp_file) {
        std::cout << "Error creating temporary file." << std::endl;
        file.close();
        return;
    }
    Book book;
    std::string line;
    bool found = false;
    while (std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::getline(line_stream, book.title, '#');
        std::getline(line_stream, book.author, '#');
        line_stream >> book.quantity;
        if (book.title == title && book.author == author) {
            found = true;
            continue;
        }
        temp_file << book.title << "#" << book.author << "#" << book.quantity << "\n";
    }
    file.close();
    temp_file.close();
    if (!found) {
        std::remove(TEMP_BOOKS_PATH);
        std::cout << "Book not found." << std::endl;
        return;
    }
    std::remove(BOOKS_PATH); // On my system for some reason std::rename doesn't work (doesn't overwrite) if I don't remove the file first
    if (std::rename(TEMP_BOOKS_PATH, BOOKS_PATH) != 0) {
        std::cout << "Error updating books file." << std::endl;
        return;
    }
    std::cout << "Book removed successfully." << std::endl;
}

void viewIssuedBooksForInputStudent() {
    std::string roll_number;
    std::cout << "Enter student roll number to view issued books: "; std::cin >> roll_number;

    viewIssuedBooks(roll_number);
}

void handleAdminSession(const std::string& username) {
    std::cout << "Welcome, " << username << "." << std::endl;

    while (true) {
        std::cout << R"(Please choose an option:
            1. View all books.
            2. Search for a book.
            3. Add books.
            4. Remove books.
            5. View issued books for a particular student.
            9. Log out.
        )" << std::endl;
        int choice; std::cin >> choice;
        switch (choice) {
            case 1:
                showBooks();
                break;
            case 2:
                searchBook();
                break;
            case 3:
                addBook();
                break;
            case 4:
                removeBook();
                break;
            case 5:
                viewIssuedBooksForInputStudent();
                break;
            case 9:
                std::cout << "Logging out..." << std::endl;
                return; // Exit the program
            default:
                std::cout << "Invalid choice." << std::endl;
                break;
        }
    }
}