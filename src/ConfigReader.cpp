//
// Created by tobias on 03.12.19.
//

#include "ConfigReader.h"

void ConfigReader::readConfig(const std::string &filename) {
    std::ifstream file;
    file.open(filename);
    if (!file) {
        throw std::invalid_argument("File " + filename + " could not be opened");
    }
    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        std::istringstream tmp(line);
        std::string attr;
        if (std::getline(tmp, attr, '=')) {
            std::string val;
            if (std::getline(tmp, val)) {
                values.emplace(attr, val);
            }
        } else {
            throw std::invalid_argument("Attributes and Values must be separated by an equal sign without enclosing whitespaces! This is not the case in line " +
                                        std::to_string(lineNumber));
        }
    }
    file.close();
}

std::string ConfigReader::getVal(const std::string &attrName) const {
    if (values.find(attrName) != values.end()) {
        return values.find(attrName)->second;
    }
    throw AttributeNotFoundException(attrName);
}

int ConfigReader::getIntVal(const std::string &attrName) const {
    if (values.find(attrName) != values.end()) {
        return std::stoi(values.find(attrName)->second);
    }
    throw AttributeNotFoundException(attrName);
}

bool ConfigReader::getBoolVal(const std::string &attrName) const {
    if (values.find(attrName) != values.end()) {
        return std::stoi(values.find(attrName)->second) == 1;
    }
    throw AttributeNotFoundException(attrName);
}

double ConfigReader::getDoubleVal(const std::string &attrName) const {
    if (values.find(attrName) != values.end()) {
        return std::stod(values.find(attrName)->second);
    }
    throw AttributeNotFoundException(attrName);
}