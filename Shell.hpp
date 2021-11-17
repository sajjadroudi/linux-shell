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

    void executeCommand(const string& command) {
        executor->execute(command);
    }

private:
    string getDefaultPath() const {
        struct passwd *pw = getpwuid(getuid());
        return string{pw->pw_dir};
    }

};