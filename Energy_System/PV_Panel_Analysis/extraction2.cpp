#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Function to check if a character is a digit or a decimal point
bool isNumeric(char c) {
    return (isdigit(c) || c == '.');
}

// Function to extract numbers from a line of text
std::string extractNumbers(const std::string& line) {
    std::stringstream ss(line);
    std::string word, numbers;

    while (ss >> word) {
        std::string numStr;
        for (char c : word) {
            if (isNumeric(c)) {
                numStr += c;
            }
        }

        if (!numStr.empty()) {
            numbers += numStr + " ";
        }
    }

    return numbers;
}

int main() {
    std::string inputFilename = "data.txt";
    std::string outputFilename = "output2.txt";

    std::ifstream inputFile(inputFilename);
    if (inputFile.is_open()) {
        std::ofstream outputFile(outputFilename);
        if (outputFile.is_open()) {
            std::string line;
            while (std::getline(inputFile, line)) {
                std::string numbers = extractNumbers(line);
                if (!numbers.empty()) {
                    outputFile << numbers << std::endl;
                }
            }
            outputFile.close();
            std::cout << "Output file created successfully: " << outputFilename << std::endl;
        } else {
            std::cerr << "Unable to create output file: " << outputFilename << std::endl;
        }
        inputFile.close();
    } else {
        std::cerr << "Unable to open input file: " << inputFilename << std::endl;
    }

    return 0;
}
