#include "CampusCompass.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>

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

bool CampusCompass::insertStudent(std::string &name, int &UFID, int &residenceLoc, std::set<std::string> &classes) {
    if (students.find(UFID) != students.end()) {
        return false;
    }
    Student s{UFID, name, residenceLoc, classes};
    students[s.UFID] = s;
    return true;
}

bool CampusCompass::removeStudent(int UFID) {
    size_t success = students.erase(UFID);
    if (success == 0) {
        return false;
    }
    return true;
}

bool CampusCompass::dropClass(int &UFID, std::string &classCode) {
    if (students.find(UFID) == students.end()) {
        return false;
    }
    if (classCodes.find(classCode) == classCodes.end()) {
        return false;
    }
    Student& s = students.at(UFID);
    size_t success = s.classes.erase(classCode);
    return success == 1;
}

bool CampusCompass::replaceClass(int &UFID, std::string &classCode1, std::string &classCode2) {
    if (students.find(UFID) == students.end()) {
        return false;
    }
    Student& s = students.at(UFID);
    if (classCodes.find(classCode2) == classCodes.end()) {
        return false;
    }

    if (s.classes.find(classCode1) == s.classes.end() ||
        s.classes.find(classCode2) != s.classes.end()) {
        return false;
    }
    s.classes.erase(classCode1);
    s.classes.insert(classCode2);
    if (s.classes.size() == 0) {
        students.erase(UFID);
    }
    return true;
}

int CampusCompass::removeClass(std::string &classCode) {
    if (classCodes.find(classCode) == classCodes.end()) {
        return 0;
    }
    int count = 0;
    for (auto iter = students.begin(); iter != students.end(); ++iter) {
        Student& s = iter->second;
        if (s.classes.find(classCode) == s.classes.end()) {
            continue;
        }
        s.classes.erase(classCode);
        if (s.classes.size() == 0) {
            students.erase(iter->first);
        }
        ++count;
    }
    return count;
}

void CampusCompass::toggleEdges(std::vector<std::pair<int, int> > &edges) {
    for (auto edge: edges) {
        for(auto & connection : connections) {
            if (edge.first == connection[0] && edge.second == connection[1]) {
                connection[3] = 1 - connection[3];
                break;
            }
        }
    }
}

std::string CampusCompass::checkEdgeStatus(std::pair<int, int> &edge) {
    for(auto & connection : connections) {
        if (edge.first == connection[0] && edge.second == connection[1]) {
            if (connection[3] == 0) {
                return "closed";
            }
            return "open";
        }
    }
    return "DNE";
}

std::vector<std::vector<int>> CampusCompass::createAdjacencyMatrix() {
    std::vector<std::vector<int>> adjacencyMatrix(connections.size(), std::vector<int>(connections.size(), 0));
    for (const auto& connection : connections) {
        if (connection[3] == 1) {
            adjacencyMatrix[connection[0]][connection[1]] = connection[2];
            adjacencyMatrix[connection[1]][connection[0]] = connection[2];
        }
    }
    return adjacencyMatrix;
}

bool CampusCompass::isConnected(std::pair<int, int> &edge) {
    int source = edge.first;
    int destination = edge.second;
    std::vector<std::vector<int>> adj= createAdjacencyMatrix();
    int V = adj.size();
    std::vector<bool> visited(V, false);
    std::queue<int> vertices;
    vertices.push(source);
    visited[source] = true;

    while (!vertices.empty()) {
        int curr = vertices.front();
        if (curr == destination) {
            return true;
        }
        vertices.pop();
        for (int i = 0; i < V; i++) {
            if (!visited[i] && adj[curr][i] >= 1) {
                visited[i] = true;
                vertices.push(i);
                if (i == destination) {
                    return true;
                }
            }
        }
    }
    return false;

}
