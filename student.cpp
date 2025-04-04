#include "main.hpp"

std::string getIssuedBookRollNumber(const Book& book) {
    std::ifstream issue_file(ISSUED_BOOKS_PATH);
    if (!issue_file) {
        std::cout << "Error opening issues file." << std::endl;
        return ""; // Return empty string if file cannot be opened
    }
    std::string roll_number;
    Book scan_book;
    std::string line;
    while (std::getline(issue_file, line)) {
        std::istringstream line_stream(line);
        std::getline(line_stream, roll_number, '#');
        std::getline(line_stream, scan_book.title, '#');
        line_stream >> scan_book.author;

        if (scan_book == book) {
            return roll_number;
        }
    }
    return ""; // Return empty string if not found
}

void issueBook(Student& student) {

    if (student.issued_books_count >= 2) {
        std::cout << "You have already issued 2 books. Return one in order to issue a new one!" << std::endl;
        return;
    }

    Book issue_book;
    std::cin.ignore();
    std::cout << "Enter book title to issue: "; std::getline(std::cin, issue_book.title);
    std::cout << "Enter book author to issue: "; std::getline(std::cin, issue_book.author);


    // Check if the book is already issued
    const std::string issued_roll_number = getIssuedBookRollNumber(issue_book);
    if (issued_roll_number == student.roll_number) {
        std::cout << "You have already issued this book." << std::endl;
        std::cout << "So, like, you want to issue the same book again? (y/n)" << std::endl;
        char choice; std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            std::cout << "Go ahead, I guess." << std::endl;
        } else if (choice == 'n' || choice == 'N') {
            std::cout << "Yeah, that's what I thought." << std::endl;
            return;
        } else {
            std::cout << "...So I am going to take that as a no." << std::endl;
            return;
        }
    }

    // Update the book quantity in the books file
    std::ifstream books_file(BOOKS_PATH);
    if (!books_file) {
        std::cout << "Error opening books file." << std::endl;
        return;
    }
    std::ofstream temp_books_file(TEMP_BOOKS_PATH);
    if (!temp_books_file) {
        std::cout << "Error creating temporary file." << std::endl;
        books_file.close();
        return;
    }
    Book scan_book;
    bool found = false;
    std::string line;
    while (std::getline(books_file, line)) {
        std::istringstream line_stream(line);
        std::getline(line_stream, scan_book.title, '#');
        std::getline(line_stream, scan_book.author, '#');
        line_stream >> scan_book.quantity;

        if (scan_book == issue_book) {
            if (scan_book.quantity <= 0) {
                std::cout << "Book not available for issuing. We ran out of them." << std::endl;
                books_file.close();
                temp_books_file.close();
                std::remove(TEMP_BOOKS_PATH);
                return;
            }

            scan_book.quantity--;
            found = true;
        }

        temp_books_file << scan_book.title << "#" << scan_book.author << "#" << scan_book.quantity << "\n";
    }
    books_file.close();
    temp_books_file.close();
    if (!found) {
        std::cout << "We don't have that book." << std::endl;
        std::remove(TEMP_BOOKS_PATH);
        return;
    }
    std::remove(BOOKS_PATH); // On my system std::rename doesn't overwrite the file if it exists for some reason, so I have to remove it first
    if (std::rename(TEMP_BOOKS_PATH, BOOKS_PATH) != 0) {
        std::cout << "Error updating books file." << std::endl;
        return;
    }


    // Append issued books file
    std::ofstream issue_file(ISSUED_BOOKS_PATH, std::ios::app);
    if (!issue_file) {
        std::cout << "Error opening issues file." << std::endl;
        return;
    }
    issue_file << student.roll_number << "#" << issue_book.title << "#" << issue_book.author << "#" << "\n";
    issue_file.close();


    // Update students.csv file to increment issued books count
    std::ifstream student_file(USER_PATH);
    if (!student_file) {
        std::cout << "Error opening student file." << std::endl;
        return;
    }
    std::ofstream temp_student_file(TEMP_USER_PATH);
    if (!temp_student_file) {
        std::cout << "Error creating temporary student file." << std::endl;
        student_file.close();
        return;
    }

    Student scan_student;
    bool student_found = false;
    while (std::getline(student_file, line)) {
        std::istringstream line_stream(line);
        std::getline(line_stream, scan_student.roll_number, '#');
        std::getline(line_stream, scan_student.password, '#');
        line_stream >> scan_student.issued_books_count;

        if (scan_student.roll_number == student.roll_number) {
            scan_student.issued_books_count++;
            student_found = true;
        }

        temp_student_file << scan_student.roll_number << "#" << hashPassword(scan_student.password) << "#" << scan_student.issued_books_count << "\n";
    }

    student_file.close();
    temp_student_file.close();

    if (!student_found) {
        std::cout << "Error: Student not found in the file." << std::endl;
        return;
    }

    // Check if the temporary file exists
    std::ifstream temp_file_check(TEMP_USER_PATH);
    if (!temp_file_check) {
        std::cout << "Error: Temporary student file was not created successfully." << std::endl;
        return;
    }
    temp_file_check.close();

    // Attempt to remove the original file
    if (std::remove(USER_PATH) != 0) {
        perror("Error removing USER_PATH");
    }

    // Attempt to rename the temporary file
    if (std::rename(TEMP_USER_PATH, USER_PATH) != 0) {
        perror("Error renaming TEMP_USER_PATH to USER_PATH");
        return;
    }

    std::cout << "Student file updated successfully." << std::endl;


    std::cout << "Book issued successfully." << std::endl;
    student.issued_books_count++;
}

void returnBook(Student& student) {

    if (student.issued_books_count <= 0) {
        std::cout << "You haven't issued any books." << std::endl;
        return;
    }

    Book return_book;
    std::cin.ignore();
    std::cout << "Enter book title to return: "; std::getline(std::cin, return_book.title);
    std::cout << "Enter book author to return: "; std::getline(std::cin, return_book.author);


    // Check if the book is issued
    if (getIssuedBookRollNumber(return_book) != student.roll_number) {
        std::cout << "You haven't even issued this book bro, whose book are you returning??" << std::endl;
        return;
    }


    // Update the book quantity in the books file
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
    bool found = false;
    std::string line;
    while (std::getline(books_file, line)) {
        std::istringstream line_stream(line);
        std::getline(line_stream, scan_book.title, '#');
        std::getline(line_stream, scan_book.author, '#');
        line_stream >> scan_book.quantity;

        if (scan_book == return_book) {
            scan_book.quantity++;
            found = true;
        }

        temp_books_file << scan_book.title << "#" << scan_book.author << "#" << scan_book.quantity << "\n";
    }
    books_file.close();
    temp_books_file.close();
    if (!found) {
        std::cout << "Book not found." << std::endl;
        return;
    }
    std::remove(BOOKS_PATH); // On my system std::rename doesn't overwrite the file if it exists for some reason, so I have to remove it first
    if (std::rename(TEMP_BOOKS_PATH, BOOKS_PATH) != 0) {
        std::cout << "Error updating books file." << std::endl;
        return;
    }


    // Remove the book from the issued books file
    std::ifstream issue_file(ISSUED_BOOKS_PATH);
    if (!issue_file) {
        std::cout << "Error opening issues file." << std::endl;
        return;
    }
    std::ofstream temp_issue_file(TEMP_ISSUED_BOOKS_PATH);
    if (!temp_issue_file) {
        std::cout << "Error creating temporary issues file." << std::endl;
        issue_file.close();
        return;
    }
    //std::string line;
    while (std::getline(issue_file, line)) {
        if (line.find(student.roll_number) == std::string::npos || line.find(return_book.title) == std::string::npos || line.find(return_book.author) == std::string::npos) {
            temp_issue_file << line << "\n";
        }
    }
    issue_file.close();
    temp_issue_file.close();
    std::remove(ISSUED_BOOKS_PATH); // On my system std::rename doesn't overwrite the file if it exists for some reason, so I have to remove it first
    if (std::rename(TEMP_ISSUED_BOOKS_PATH, ISSUED_BOOKS_PATH) != 0) {
        std::cout << "Error updating issues file." << std::endl;
        return;
    }


    // Update students.csv file to decrement issued books count
    std::ifstream student_file(USER_PATH, std::ios::app);
    if (!student_file) {
        std::cout << "Error opening student file." << std::endl;
        return;
    }
    std::ofstream temp_student_file(TEMP_USER_PATH);
    if (!temp_student_file) {
        std::cout << "Error creating temporary student file." << std::endl;
        student_file.close();
        return;
    }
    Student scan_student;
    //std::string line;
    while (std::getline(student_file, line)) {
        std::istringstream line_stream(line);
        std::getline(line_stream, scan_student.roll_number, '#');
        std::getline(line_stream, scan_student.password, '#');
        line_stream >> scan_student.issued_books_count;
        if (scan_student.roll_number == student.roll_number) {
            scan_student.issued_books_count--;
        }

        temp_student_file << scan_student.roll_number << "#" << hashPassword(scan_student.password) << "#" << scan_student.issued_books_count << "\n";
    }
    student_file.close();
    temp_student_file.close();
    // not handling the student not found case because that would be... weird. why would that even happen
    std::remove(USER_PATH); // On my system std::rename doesn't overwrite the file if it exists for some reason, so I have to remove it first
    if (std::rename(TEMP_USER_PATH, USER_PATH) != 0) {
        std::cout << "Error updating student file." << std::endl;
        return;
    }

    std::cout << "Book returned successfully." << std::endl;
    student.issued_books_count--;
    std::cout << "You have " << student.issued_books_count << " books issued." << std::endl;
}

void handleStudentSession(Student& student) {
    std::cout << "Welcome, student!" << std::endl;
    std::cout << "Currently logged in as: " << student.roll_number << std::endl;

    while (true) {
        std::cout << R"(Please choose an option:
            1. List all books.
            2. Search for a book.
            3. Issue a book.
            4. Return a book.
            5. View issued books.
            9. Logout.
        )"<< std::endl;
        int choice; std::cin >> choice;

        switch (choice) {
            case 1:
                showBooks();
                break;
            case 2:
                searchBook();
                break;
            case 3:
                issueBook(student);
                break;
            case 4:
                returnBook(student);
                break;
            case 5:
                viewIssuedBooks(student.roll_number);
                break;
            case 9:
                std::cout << "You have now logged out." << std::endl;
                return; // Exit the program
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}