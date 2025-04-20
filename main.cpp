#include <iostream>
#include "MaxHeap.h"
#include "BPlusTree.cpp"
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

// change input cols here
const std::vector<int> COLUMNS_TO_KEEP = {0, 1, 2, 5}; // Selecting specific columns: id, type, title, year

// desired format:[id, name, year,rating]
// Function to read and combine data from two TSV files: one for movie metadata, one for ratings
std::vector<std::vector<std::string>> readTSV(const std::string& filename, const std::string ratingsFile, int maxRows = 1000000) {
    std::ifstream file(filename); // Open main movie data file
    std::vector<std::vector<std::string>> data;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return data;
    }

    int rowCount = 0;
    std::getline(file, line); // just to remove first row (header)
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> row;
        std::vector<std::string> allColumns;

        // tab separated
        while (std::getline(ss, token, '\t')) {
            allColumns.push_back(token); // Read all columns
        }

        // Keep only columns you need
        for (int index : COLUMNS_TO_KEEP) {
            if (index < allColumns.size())
                row.push_back(allColumns[index]); // Extract required fields
        }

        data.push_back(row); // Store filtered row
        rowCount++;

        if (maxRows > 0 && rowCount >= maxRows) break; // Stop after maxRows if specified
    }
    file.close();

    // Open ratings file
    std::ifstream rFile(ratingsFile);
    if (!rFile.is_open()) {
        std::cerr << "Error: Could not open file " << ratingsFile << "\n";
    }

    // ratings are index 1
    rowCount = 0;
    std::getline(rFile, line); // remove filler line (header)
    while (std::getline(rFile, line)) {
        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> row;
        std::vector<std::string> allColumns;

        // tab separated
        for (int i = 0; i < 3; i++) {
            std::getline(ss, token, '\t');
            // we are adding the rating to the last index of the vector
            if (i == 1)
                data[rowCount].push_back(token); // Add rating to corresponding movie row
        }

        rowCount++;
        if (maxRows > 0 && rowCount >= maxRows) break;
    }

    return data; // Return combined dataset
}

int main() {
    std::cout << "Input year to search" << std::endl;
    std::string inputYear;
    std::cin >> inputYear;

    MaxHeap maxHeap; // MaxHeap to store movies by rating
    BPlusTree tree;  // B+ Tree to index movies by year

    std::string filename = "/Users/Aidan/Downloads/title-basics.txt";
    std::string ratingsFile = "/Users/Aidan/Downloads/title-ratings.txt";

    auto data = readTSV(filename, ratingsFile); // Read and merge input files

    // Make entries of input year into movie objects. insert into maxHeap.
    for (const auto& row : data) {
        // Filter by year and type (must be a movie)
        if (row[3] == inputYear && row[1] == "movie") {
            // Create a new Movie object from data row
            Movie* m = new Movie(row[0], row[2], std::stod(row[4]), std::stoi(row[3]));
            maxHeap.insertPub(*m); // Insert into maxHeap (copy)
            tree.insert(m);        // Insert into B+ Tree (pointer)
        }
    }

    // Print top 10 movies by rating using MaxHeap
    std::cout << "Top 10 Movies from Heap in " << inputYear << std::endl;
    maxHeap.printTop5();
    std::cout << std::endl;

    // Print top 10 movies by rating using B+ Tree
    std::cout << "Top 10 Movies from B Tree in " << inputYear << std::endl;
    vector<Movie*> results = tree.query(stoi(inputYear));
    for (auto* movie : results) {
        cout << movie->TITLE << " (" << movie->RATING << ")" << endl;
    }

    return 0;
}
``
