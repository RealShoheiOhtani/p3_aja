#include <iostream>
#include <sstream>>
#include <fstream>
#include <vector>
#include <string>

// change input cols here
const std::vector<int> COLUMNS_TO_KEEP = {1, 2, 3};

std::vector<std::vector<std::string>> readTSV(const std::string& filename, int maxRows = 10000) {
    std::ifstream file(filename);
    std::vector<std::vector<std::string>> data;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return data;
    }

    int rowCount = 0;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> row;
        std::vector<std::string> allColumns;

        // tab separated
        while (std::getline(ss, token, '\t')) {
            allColumns.push_back(token);
        }

        // Keep only columns in vec
        for (int index : COLUMNS_TO_KEEP) {
            if (index < allColumns.size())
                row.push_back(allColumns[index]);
        }

        data.push_back(row);
        rowCount++;

        if (maxRows > 0 && rowCount >= maxRows) break;
    }

    return data;
}
int main() {
    std::string filename = "name.basics.tsv";
    auto data = readTSV(filename);

    // Print first entries
    for (const auto& row : data) {
        for (const auto& col : row)
            std::cout << col << " ";
        std::cout << "\n";
    }

    return 0;
}
