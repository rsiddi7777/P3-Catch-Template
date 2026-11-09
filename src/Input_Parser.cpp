//
// Created by rsidd on 11/8/2026.
//

#include "Input_Parser.h"

#include <iomanip>
#include <iostream>
#include <ostream>

//Selects Proper Command
std::string Input_Parser::selectCommand(std::string input) {
    const std::size_t firstSpace = input.find(" ");
    std::string command;
    std::string parameters= " ";
    //std::cout << input << std::endl;
    if (firstSpace != std::string::npos) {
        command = input.substr(0, firstSpace);
        parameters = input.substr(firstSpace+1);

    }else {
        command = input;
    }
    auto it = commands.find(command);
    if (it != commands.end()) {
        return it->second(parameters);
    }else {
        return "unsuccessful";
    }
}

std::string Input_Parser::insert(std::string command) {
    std::stringstream ss(command);
    std::string name;
    int residentLoc;
    std::string UFID;
    int N;
    ss>>std::quoted(name) >> UFID >> residentLoc >> N;
    if (!validateName(name) || !validateID(UFID))
        return "unsuccessful";

    std::set<std::string> classCodes;
    for (int i = 0; i < N; i++) {
        std::string classCode;
        if (!(ss>>classCode))
            return "unsuccessful";
        if (!validateClass(classCode))
            return "unsuccessful";
        classCodes.insert(classCode);
    }
    std::string checkExtra;
    if (ss>>checkExtra)
        return "unsuccessful";
    int ID = std::stoi(UFID);
    if (graph_->insertStudent(name, ID, residentLoc, classCodes))
        return "successful";
    return "unsuccessful";


}

std::string Input_Parser::remove(std::string command) {
    if (!validateID(command)) {
        return "unsuccessful";
    }
    int UFID = std::stoi(command);
    if (graph_->removeStudent(UFID))
        return "successful";
    return "unsuccessful";
}

std::string Input_Parser::dropClass(std::string command) {
    std::stringstream ss(command);
    std::string UFID;
    std::string classCode;
    ss >> UFID >> classCode;

    if (!validateClass(classCode) || !validateID(UFID))
        return "unsuccessful";

    int ID = std::stoi(UFID);
    if (graph_->dropClass(ID, classCode))
        return "successful";
    return "unsuccessful";
}

std::string Input_Parser::replaceClass(std::string command) {
    std::stringstream ss(command);
    std::string UFID;
    std::string classCode1;
    std::string classCode2;
    ss >> UFID >> classCode1 >> classCode2;
    if (!validateClass(classCode2) || !validateClass(classCode1) || !validateName(UFID))
        return "unsuccessful";
    int ID = std::stoi(UFID);
    if (graph_->replaceClass(ID, classCode1, classCode2))
        return "successful";
    return "unsuccessful";
}

std::string Input_Parser::removeClass(std::string command) {
    std::string classCode = command;
    if (!validateClass(classCode))
        return "unsuccessful";
    int dropped = graph_->removeClass(classCode);
    if (dropped == 0)
        return "unsuccessful";
    return std::to_string(dropped);
}

std::string Input_Parser::toggleEdgeClosure(std::string command) {
    int N;
    std::stringstream ss(command);
    ss >> N;
    std::vector<std::pair<int,int>> edges;
    for (int i = 0; i < N; i++) {
        std::pair<int,int> edge;
        ss >>edge.first >> edge.second;
        edges.push_back(edge);
    }
    graph_->toggleEdges(edges);
    return "successful";
}

std::string Input_Parser::checkEdgeStatus(std::string command) {
    std::stringstream ss(command);
    std::pair<int,int> edge;

    ss >> edge.first >> edge.second;

    return graph_->checkEdgeStatus(edge);
}

std::string Input_Parser::isConnected(std::string command) {
    std::stringstream ss(command);
    std::pair<int,int> edge;
    ss >> edge.first >> edge.second;
    if (graph_->isConnected(edge))
        return "successful";
    return "unsuccessful";
}

std::string Input_Parser::printShortestEdges(std::string command) {
    if (!validateID(command))
        return "unsuccessful";
    int UFID = std::stoi(command);
    return graph_->printShortestEdges(UFID);
}

std::string Input_Parser::printStudentZone(std::string command) {
    if (!validateID(command))
        return "unsuccessful";
    int UFID = std::stoi(command);
    return graph_->printStudentZone(UFID);
}

//Validates ID
bool Input_Parser::validateID(std::string ID) {
    std::regex pattern("^[0-9]{8}$");
    return std::regex_match(ID, pattern);
}

//Validates Name
bool Input_Parser::validateName(std::string name) {
    std::regex pattern("^[A-Za-z ]+$");
    return std::regex_match(name, pattern);
}

bool Input_Parser::validateClass(std::string classCode) {
    std::regex classCodePattern("^[A-Z]{3}[0-9]{4}$");
    return std::regex_match(classCode, classCodePattern);
}
