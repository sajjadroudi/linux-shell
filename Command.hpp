#pragma once

#include <string>
#include <vector>
#include "utils.hpp"

using namespace std;

class Command {

private:
    vector<char**>* commands = new vector<char**>;

public:
    Command(const string& command) {
        auto pipelineCommands = split(command, "|");
        for(auto & cmd : *pipelineCommands) {
            char** standardFormatCommand = convertToStandardFormat(trim(cmd));
            commands->push_back(standardFormatCommand);
        }
    }

    char** getCommand(int index) const {
        return commands->at(index);
    }

    int commandCount() const {
        return commands->size();
    }

    bool isChangeDirectoryCommand() const {
        char* cmd = getCommand(0)[0];
        return commandCount() == 1 && strcmp(cmd, "cd") == 0;
    }

private:
    static char** convertToStandardFormat(const string& command) {
        auto pieces = split(command, " ");
        char** args = toCharArrayArray(*pieces);
        args = appendNull(args, pieces->size());
        return args;
    }

};