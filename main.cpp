#include <iostream>
#include "MaxHeap.h"
#include "BPlusTree.cpp"
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

// change input cols here
const std::vector<int> COLUMNS_TO_KEEP = {0, 1, 2, 5};
// desired format:[id, name, year,rating]
std::vector<std::vector<std::string>> readTSV(const std::string& filename,const std::string ratingsFile, int maxRows = 1000000) {

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
int main() {
    std::cout<<"Input year to search"<<std::endl;
    std::string inputYear;
    std::cin>>inputYear;
    MaxHeap maxHeap;
    BPlusTree tree;

    std::string filename = "/Users/Aidan/Downloads/title-basics.txt";
    std::string ratingsFile = "/Users/Aidan/Downloads/title-ratings.txt";

    auto data = readTSV(filename, ratingsFile);

    //Make entries of input year into movie objects. insert into maxHeap.
    for (const auto& row : data) {
        if (row[3]==inputYear && row[1]== "movie") {
            Movie *m = new Movie(row[0], row[2], std::stod(row[4]), std::stoi(row[3]));
            maxHeap.insertPub(*m);
            tree.insert(m);

        }
    }
    std::cout<<"Top 10 Movies from Heap in " << inputYear << std::endl;
    maxHeap.printTop5();
    std::cout<<std::endl;
    std::cout<<"Top 10 Movies from B Tree in " << inputYear << std::endl;
    vector<Movie*> results = tree.query(stoi(inputYear));
    for (auto* movie : results) {
        cout << movie->TITLE << " (" << movie->RATING << ")" << endl;
    }
    return 0;
}