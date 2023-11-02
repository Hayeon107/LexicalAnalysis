#include <fstream>
#include <iostream>
#include <string>

int main() {
    std::ifstream file("front.txt");
    if (file.is_open()) {
        std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Display the code to verify that it has been read correctly
        std::cout << "Code from file:\n" << code << std::endl;

        // Now, you can interpret or execute the code as needed.
        // This might involve tokenizing, parsing, and running the code.

        // Example: Just printing the code
        std::cout << "Executing the code:" << std::endl;
        std::cout << code;
    }
    else {
        std::cerr << "Error: Unable to open the file." << std::endl;
    }

    return 0;
}
