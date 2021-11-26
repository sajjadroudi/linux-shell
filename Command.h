#pragma once

#include <string>
#include <vector>
#include "utils.h"
#include "AliasHandler.h"

using namespace std;

class Command {

private:
    vector<char**>* commands = new vector<char**>;
    vector<string>* strCommands = new vector<string>;

public:
    Command(const string& command) {
        AliasHandler* aliasHandler = AliasHandler::getInstance();

        auto pipelineCommands = split(command, "|");
        for(auto cmd : *pipelineCommands) {
            cmd = aliasHandler->applyAlias(trim(cmd));

            strCommands->push_back(cmd);

            char** standardFormatCommand = convertToStandardFormat(cmd);
            commands->push_back(standardFormatCommand);
        }

    }

    char** getCommand(int index) const {
        return commands->at(index);
    }

    string getStringCommand(int index) const {
        return strCommands->at(index);
    }

    int commandCount() const {
        return commands->size();
    }

    bool isChangeDirectoryCommand() const {
        return isCommand("cd");
    }

    bool isFileExecutionCommand() const {
        return isCommand("fshell");
    }

    bool isExitCommand() const {
        return isCommand("exit");
    }

    bool isAliasCommand() const {
        return isCommand("alias");
    }

    bool isUnaliasCommand() const {
        return isCommand("unalias");
    }

private:
    static char** convertToStandardFormat(const string& command) {
        auto pieces = split(command, " ");
        char** args = toCharArrayArray(*pieces);
        args = appendNull(args, pieces->size());
        return args;
    }

    bool isCommand(const string&& command) const {
        char* cmd = getCommand(0)[0];
        return commandCount() == 1 && strcmp(cmd, command.c_str()) == 0;
    }

};