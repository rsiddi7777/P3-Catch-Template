#include "CampusCompass.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>

CampusCompass::CampusCompass() {
    connections.clear();
    classCodes.clear();
    students.clear();
    vertexCount = 0;
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
        if (std::stoi(row[0])+1 >vertexCount) {
            vertexCount = std::stoi(row[0])+1;
        }
        if (std::stoi(row[1])+1 >vertexCount) {
            vertexCount = std::stoi(row[1])+1;
        }
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
    std::vector<std::vector<int>> adjacencyMatrix(vertexCount, std::vector<int>(vertexCount, 0));
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
    int V = vertexCount;
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

std::string CampusCompass::printShortestEdges(int &UFID) {
    const Student& s = students.at(UFID);
    std::string output = "Time For Shortest Edges: " + s.name + "\n";
    std::vector<int> travelTimes = shortestPath(s.residenceLoc);
    for (const std::string& classCode : s.classes) {
        int destVertex = classCodes[classCode];
        int travelTime = -1;
        if (travelTimes[destVertex] < INT_MAX) {
            travelTime = travelTimes[destVertex];
        }
        output += classCode + ": " + std::to_string(travelTime) + "\n";
    }
    output.pop_back();
    return output;
}

std::string CampusCompass::printStudentZone(int &UFID) {
    const Student& s = students.at(UFID);
    std::string output = "Student Zone Cost for: " + s.name;
    std::vector<int> travelTimes = shortestPath(s.residenceLoc);
    std::set<int> accessible;
    for (const std::string& classCode : s.classes) {
        int destVertex = classCodes[classCode];
        if (travelTimes[destVertex] < INT_MAX) {
            accessible.insert(destVertex);
        }
    }
    std::set<int> pathVertices(accessible);
    std::vector<int> pathTrace = shortestVertices(s.residenceLoc);
    for (int vertex: accessible) {
        int curr = pathTrace[vertex];
        while (pathTrace[curr] != -1) {
            pathVertices.insert(curr);
            curr = pathTrace[curr];
        }
    }
    std::vector<int>parent(vertexCount);
    for (int i = 0; i<vertexCount; i++) {
        parent[i] = i;
    }
    std::vector<std::vector<int>> draftMST;
    for (std::vector<int> connection : connections) {
        if (pathVertices.find(connection[0]) != pathVertices.end() &&
            pathVertices.find(connection[1]) != pathVertices.end() &&
            connection[3] == 1) {
                draftMST.push_back(connection);
        }
    }
    std::sort(draftMST.begin(), draftMST.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        return a[2] < b[2];
    });
    std::vector<std::vector<int>> MST;
    int zoneSum = 0;
    for (std::vector<int> edge: draftMST) {
        if (find(edge[0], parent)!=find(edge[1], parent)) {}
            MST.push_back(edge);
            zoneSum+=edge[2];



    }


    return output + " " + std::to_string(zoneSum);
}

std::vector<int> CampusCompass::shortestPath(int source) {
    std::vector<std::vector<int>> adjacencyMatrix = createAdjacencyMatrix();
    std::set<int> S;
    std::set<int> VS;
    std::vector<int> d(vertexCount,INT_MAX);
    std::vector<int> p(vertexCount,-1);
    S.insert(source);
    d[source] = 0;
    for (int i = 1; i < vertexCount; i++) {
        if (i!=source) {
            VS.insert(i);
        }
    }
    for (int vertex: VS) {
        if (adjacencyMatrix[source][vertex] >0) {
            d[vertex] = adjacencyMatrix[source][vertex];
            p[vertex] = source;
        }else {
            d[vertex] = INT_MAX;
        }
    }
    while (!VS.empty()) {
        int smallest = INT_MAX;
        int u = 0;
        for (int vertex: VS) {
            if (d[vertex] < smallest) {
                smallest = d[vertex];
                u = vertex;
            }
        }
        if (smallest == INT_MAX)
            return d;

        VS.erase(u);
        S.insert(u);
        for (int vertex: VS) {
            if (adjacencyMatrix[vertex][u] >0 && d[u] < INT_MAX) {
                if (d[u] + adjacencyMatrix[vertex][u] < d[vertex]) {
                    d[vertex] = d[u] + adjacencyMatrix[vertex][u];
                    p[vertex] = u;
                }
            }
        }
    }
    return d;
}


std::vector<int> CampusCompass::shortestVertices(int source) {
    std::vector<std::vector<int>> adjacencyMatrix = createAdjacencyMatrix();
    std::set<int> S;
    std::set<int> VS;
    std::vector<int> d(vertexCount,INT_MAX);
    std::vector<int> p(vertexCount,-1);
    S.insert(source);
    d[source] = 0;
    for (int i = 1; i < vertexCount; i++) {
        if (i!=source) {
            VS.insert(i);
        }
    }
    for (int vertex: VS) {
        if (adjacencyMatrix[source][vertex] >0) {
            d[vertex] = adjacencyMatrix[source][vertex];
            p[vertex] = source;
        }else {
            d[vertex] = INT_MAX;
        }
    }
    while (!VS.empty()) {
        int smallest = INT_MAX;
        int u = 0;
        for (int vertex: VS) {
            if (d[vertex] < smallest) {
                smallest = d[vertex];
                u = vertex;
            }
        }
        if (smallest == INT_MAX)
            return p;

        VS.erase(u);
        S.insert(u);
        for (int vertex: VS) {
            if (adjacencyMatrix[vertex][u] >0 && d[u] < INT_MAX) {
                if (d[u] + adjacencyMatrix[vertex][u] < d[vertex]) {
                    d[vertex] = d[u] + adjacencyMatrix[vertex][u];
                    p[vertex] = u;
                }
            }
        }
    }
    return p;
}

int CampusCompass::find(int i, std::vector<int>& parent) {
    if (i == parent[i])
        return i;

    return find(parent[i], parent);
}

void CampusCompass::union_set(int i, int j, std::vector<int>& parent) {
    parent[i] = parent[j];
}
