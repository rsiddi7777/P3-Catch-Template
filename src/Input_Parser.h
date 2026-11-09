//
// Created by rsidd on 11/8/2026.
//

#ifndef PROJECT3_INPUT_PARSER_H
#define PROJECT3_INPUT_PARSER_H


#include <array>
#include <functional>
#include <string>
#include <map>
#include <unordered_map>
#include <utility>
#include "CampusCompass.h"
#include <regex>
class Input_Parser {
private:
    CampusCompass* graph_;
    int numCommands_;
    std::string insert(std::string command);
    std::string remove(std::string command);
    std::string dropClass(std::string command);
    std::string replaceClass(std::string command);
    std::string removeClass(std::string command);
    std::string toggleEdgeClosure(std::string command);
    std::string checkEdgeStatus(std::string command);
    std::string isConnected(std::string command);
    std::string printShortestEdges(std::string command);
    std::string printStudentZone(std::string command);
    std::unordered_map<std::string, std::function<std::string(std::string)>> commands;

    bool validateName(std::string name);
    bool validateID(std::string ID);
    bool validateClass(std::string classCode);

public:
    explicit Input_Parser(CampusCompass* graph, int numCommands):graph_(graph), numCommands_(numCommands),
        commands{
        {"insert", [this] (std::string command) {return insert(std::move(command));}},
        {"remove",[this] (std::string command) {return remove(std::move(command));}},
        {"dropClass", [this] (std::string command) {return dropClass(std::move(command));}},
        {"replaceClass", [this] (std::string command) {return replaceClass(std::move(command));}},
        {"removeClass", [this] (std::string command) {return removeClass(std::move(command));}},
        {"toggleEdgesClosure", [this] (std::string command) {return toggleEdgeClosure(std::move(command));}},
        {"checkEdgeStatus", [this] (std::string command) {return checkEdgeStatus(std::move(command));}},
        {"isConnected", [this] (std::string command) {return isConnected(std::move(command));}},
        {"printShortestEdges", [this] (std::string command) {return printShortestEdges(std::move(command));}},
        {"printStudentZone", [this] (std::string command) {return printStudentZone(std::move(command));}},
    }
    {}

    std::string selectCommand(std::string input);


};


#endif //PROJECT3_INPUT_PARSER_H
