#include "CampusCompass.h"
#include <fstream>
#include <iostream>
#include <sstream>


CampusCompass::CampusCompass() {
    connections.clear();
    classCodes.clear();
    students.clear();
}

bool CampusCompass::ParseCSV(const std::string &edges_filepath, const std::string &classes_filepath) {
    std::ifstream edgeFile(edges_filepath);
    std::ifstream classesFile(classes_filepath);

    std::string line;
    std::getline(edgeFile, line);
    while (std::getline(edgeFile, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> row;
        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }

        //std::cout << row[0] <<", " << row[1] << ", " << row.back() << std::endl;
        std::vector edge = {std::stoi(row[0]), std::stoi(row[1]), std::stoi(row.back()), 1};
        connections.push_back(edge);
    }
    edgeFile.close();

    std::getline(classesFile, line);
    while (std::getline(classesFile, line)) {
        std::stringstream lineStream(line);
        std::string cell;
        std::vector<std::string> row;
        while (std::getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }
        //std::cout << row[0] <<", " << row[1] << std::endl;
        classCodes.insert({row[0], std::stoi(row[1])});
    }
    classesFile.close();

    return true;
}

bool CampusCompass::ParseCommand(const std::string &command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
    bool is_valid = true; // replace with your actual validity checking

    return is_valid;
}
