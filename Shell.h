#pragma once

#include "CommandExecutor.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <iostream>
#include "colormod.h"

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
                try {
                    executor->execute(line);
                } catch (...) {
                    cerr << "Invalid input" << endl;
                }
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
        Color::Modifier def(Color::FG_DEFAULT);
        Color::Modifier green(Color::FG_GREEN);
        Color::Modifier blue(Color::FG_BLUE);
        cout << green << getOutput("whoami") << "@" << getOutput("hostname")
        << def << ":" << blue << replace(getOutput("pwd"), getDefaultPath(), "~")
        << def << "$ ";
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