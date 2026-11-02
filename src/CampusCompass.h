#pragma once
#include <string>
#include <set>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

struct Student {
    int UFID;
    std::string name;
    int residenceLoc;
    std::set<std::string> classes;

    //Comparator for set usage
    bool operator<(const Student &other) const {
        return UFID < other.UFID;
    }
    bool operator<(const int searchUFID) const {
        return UFID < searchUFID;
    }
};
class CampusCompass {
private:
    std::vector<std::vector<int>> connections;
    std::map<std::string, int> classCodes;
    std::unordered_map<int, Student> students;

    bool insertStudent(std::string& name, int& UFID, int& residenceLoc, std::set<std::string>& classes);
    bool removeStudent(int UFID);
    bool dropClass(int& UFID, std::string& classCode);
    bool replaceClass(int& UFID, std::string& classCode1, std::string& classCode2);
    int removeClass(std::string& classCode);
    void toggleEdges(std::vector<std::pair<int,int>>& edges);
    std::string checkEdgeStatus(std::pair<int,int>& edge);
    bool isConnected(std::pair<int,int>& locations);
    std::string printShortestEdges(int& UFID);
    std::string printStudentZone(int& UFID);
public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const std::string &edges_filepath, const std::string &classes_filepath);
    bool ParseCommand(const std::string &command);
};
