#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Function to split a string into tokens using spaces as delimiters
std::vector<std::string> splitString(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to convert a vector of strings into a comma-separated string
std::string vectorToString(const std::vector<std::string>& vec) {
    std::stringstream ss;
    for (const std::string& str : vec) {
        ss << str << ",";
    }
    std::string result = ss.str();
    // Remove the trailing comma
    result = result.substr(0, result.length() - 1);
    return result;
}

int main() {
    std::string inputFilename = "output2.txt";
    std::string outputFilename = "output2.csv";

    std::ifstream inputFile(inputFilename);
    if (inputFile.is_open()) {
        std::ofstream outputFile(outputFilename);
        if (outputFile.is_open()) {
            std::string line;
            while (std::getline(inputFile, line)) {
                std::vector<std::string> tokens = splitString(line);
                std::string csvLine = vectorToString(tokens);
                outputFile << csvLine << std::endl;
            }
            outputFile.close();
            std::cout << "CSV file created successfully: " << outputFilename << std::endl;
        } else {
            std::cerr << "Unable to create CSV file: " << outputFilename << std::endl;
        }
        inputFile.close();
    } else {
        std::cerr << "Unable to open input file: " << inputFilename << std::endl;
    }

    return 0;
}
