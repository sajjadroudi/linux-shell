//
// Created by sajjad on 11/15/21.
//

#pragma once

#include <unistd.h>
#include "utils.hpp"

class CommandExecutor {

public:
    void execute(string command) {
        if(fork() > 0) {
            auto pieces = splitOnSpace(command);
            char** args = toCharArrayArray(*pieces);
            args = appendNull(args, pieces->size());
            execvp(args[0], args);
        }
    }
};