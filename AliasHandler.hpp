#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include "utils.hpp"
#include <pwd.h>

using namespace std;

class AliasHandler {
private:
    map<string, string>* aliases = new map<string, string>;

    AliasHandler() {}

public:
    static AliasHandler* getInstance() {
        static AliasHandler* aliasHandler = new AliasHandler;
        return aliasHandler;
    }

    string applyAlias(string command) {
        auto pieces = splitIntoTwoPieces(command, " ");
        string alias = pieces->at(0);

        if(!contains(alias))
            return command;

        string originalCommand = aliases->at(alias);
        return replace(command, alias, originalCommand);
    }

    bool contains(const string& alias) const {
        return aliases->count(alias) != 0;
    }

    void saveAlias(string alias, string command) {
        alias = trim(alias), command = trim(command);
        if(contains(alias)) {
            aliases->at(alias) = command;
        } else {
            aliases->insert(pair<string, string>(alias, command));
        }
    }

    void removeAlias(string alias) {
        aliases->erase(alias);
    }

};