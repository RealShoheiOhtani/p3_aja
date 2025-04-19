#include <iostream>
#include <sstream>>
#include <fstream>
#include <vector>
#include <string>

// change input cols here
const std::vector<int> COLUMNS_TO_KEEP = {0, 2, 5};
// desired format:[id, name, year,rating]
std::vector<std::vector<std::string>> readTSV(const std::string& filename,const std::string ratingsFile, int maxRows = 10000) {

    std::ifstream file(filename);
    std::vector<std::vector<std::string>> data;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return data;
    }

    int rowCount = 0;
    std::getline(file, line); //just to remove first row
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> row;
        std::vector<std::string> allColumns;

        // tab separated
        while (std::getline(ss, token, '\t')) {
            allColumns.push_back(token);
        }

        // Keep only columns you need
        for (int index : COLUMNS_TO_KEEP) {
            if (index < allColumns.size())
                row.push_back(allColumns[index]);
        }

        data.push_back(row);
        rowCount++;

        if (maxRows > 0 && rowCount >= maxRows) break;
    }
    file.close();


    std::ifstream rFile(ratingsFile);
    if (!rFile.is_open()) {
        std::cerr << "Error: Could not open file " << ratingsFile << "\n";
    }
    //ratings are index 1
    rowCount = 0;
    std::getline(rFile, line); //remove filler line
    while (std::getline(rFile, line)) {
        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> row;
        std::vector<std::string> allColumns;

        // tab separated
        for (int i=0;i<3;i++) {
            std::getline(ss, token, '\t');
            //we are adding the rating to the last index of the vector
            if(i==1)
                data[rowCount].push_back(token);
        }
        rowCount++;
        if (maxRows > 0 && rowCount >= maxRows) break;

    }


    return data;
}

int getValidInteger() {
    int year;
    while (true) {
        std::cout << "Enter a year: ";
        std::cin >> year;

        if (std::cin.fail()) {
            std::cin.clear(); // clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard invalid input
            std::cout << "Invalid input. Please enter a valid integer.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear any extra input
            if (year < 1900 && year < 2025) {
                return year;
            }else {
                std::cout << "Invalid input. Please enter a valid integer.\n";
            }
        }
    }
}
int main() {
    std::string filename = "/Users/Owner/Downloads/IMDb Title Basics.tsv";
    std::string ratingsFile = "/Users/Owner/Downloads/IMDb Ratings Data.tsv";

    auto data = readTSV(filename, ratingsFile,100000);

    // Print first entries
    for (const auto& row : data) {
        for (const auto& col : row)
            std::cout << col << " ";
        std::cout << "\n";
    }
    // int year=getValidInteger();

    return 0;
}
