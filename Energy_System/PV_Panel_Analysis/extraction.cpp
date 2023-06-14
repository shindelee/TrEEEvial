#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

// Function to check if a character is a digit or a decimal point
bool isNumeric(char c) {
    return (isdigit(c) || c == '.');
}

// Function to extract numerical values from a string
std::vector<double> extractNumbers(const std::string& text) {
    std::vector<double> numbers;
    std::stringstream ss(text);
    std::string word;

    while (ss >> word) {
        std::string numStr;
        for (char c : word) {
            if (isNumeric(c)) {
                numStr += c;
            }
        }

        if (!numStr.empty()) {
            double num = std::stod(numStr);
            numbers.push_back(num);
        }
    }

    return numbers;
}

// Function to write numbers to a CSV file
void writeNumbersToCSV(const std::vector<double>& numbers, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (double num : numbers) {
            file << num << ",";
        }
        file.close();
        std::cout << "CSV file created successfully: " << filename << std::endl;
    } else {
        std::cerr << "Unable to create CSV file: " << filename << std::endl;
    }
}

int main() {
    std::string inputFilename = "Boost_MPPT.txt";
    std::string outputFilename = "boost_MPPT1.csv";

    std::ifstream inputFile(inputFilename);
    if (inputFile.is_open()) {
        std::stringstream buffer;
        buffer << inputFile.rdbuf();
        std::string fileContent = buffer.str();
        inputFile.close();

        std::vector<double> numbers = extractNumbers(fileContent);
        std::sort(numbers.begin(), numbers.end());

        writeNumbersToCSV(numbers, outputFilename);
    } else {
        std::cerr << "Unable to open input file: " << inputFilename << std::endl;
    }

    return 0;
}
