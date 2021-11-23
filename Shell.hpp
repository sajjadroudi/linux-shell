#pragma once

#include "CommandExecutor.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <iostream>

using namespace std;

class Shell {

private:
    CommandExecutor* executor = new CommandExecutor;

public:
    Shell() : Shell{getDefaultPath()} {}

    Shell(const string& path) {
        executor->changeDirectory(path);
    }

    void run() {
        string line;
        printPrompt();
        while(true) {
            getline(cin, line);
            if(!line.empty()) {
                executor->execute(line);
            }
            printPrompt();
        }
    }

private:
    string getDefaultPath() const {
        struct passwd *pw = getpwuid(getuid());
        return string{pw->pw_dir};
    }

    void printPrompt() {
        string prompt = getOutput("whoami") + "@" +
            getOutput("hostname") + ":" +
            replace(getOutput("pwd"), getDefaultPath(), "~") + "$ ";

        cout << prompt;
    }

    string getOutput(string command) {
        return executor->getOutput(Command(command));
    }

};