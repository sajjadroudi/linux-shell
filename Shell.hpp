#pragma once

#include "CommandExecutor.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <iostream>
#include "colormod.hpp"

using namespace std;

class Shell {

private:
    CommandExecutor* executor = new CommandExecutor;

    Shell() : Shell{getDefaultPath()} {}

    Shell(const string& path) {
        executor->changeDirectory(const_cast<char *>(path.c_str()));
    }

public:
    static Shell* getInstance() {
        static Shell* instance = new Shell;
        return instance;
    }

    void run() {
        registerCtrlCHandler();

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

        Color::Modifier green(Color::FG_GREEN);
        Color::Modifier def(Color::FG_DEFAULT);
        cout << green << prompt << def;
    }

    string getOutput(string command) {
        return executor->getOutput(Command(command));
    }

    void registerCtrlCHandler() {
        signal(SIGINT, [](int s) -> void {
            cout << endl;
            getInstance()->printPrompt();
            cout << flush;
        });
    }

};