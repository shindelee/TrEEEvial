#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

int main() {
    std::ifstream inputFile("redPowerTuned.txt");  // Replace "input.txt" with the actual file path

    if (!inputFile) {
        std::cout << "Failed to open the input file." << std::endl;
        return 1;
    }

    std::ofstream outputFile("redPowerTuned.csv");  // Replace "output.csv" with the desired output file path

    if (!outputFile) {
        std::cout << "Failed to create the output file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        std::vector<std::string> segments;
        std::istringstream iss(line);
        std::string segment;

        while (iss >> segment) {
            segments.push_back(segment);
        }

        for (const auto& entry : segments) {
            outputFile << entry << ",";
        }
        outputFile << std::endl;
    }

    inputFile.close();
    outputFile.close();

    std::cout << "CSV file created successfully." << std::endl;

    return 0;
}
