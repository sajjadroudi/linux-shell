#pragma once

#include <unistd.h>
#include "Command.hpp"

class CommandExecutor {

public:
    void execute(const string& cmd) {
        execute(Command(cmd));
    }

    void execute(const Command& cmd) {
        if(cmd.isChangeDirectoryCommand()) {
            char* path = cmd.getCommand(0)[1];
            changeDirectory(path);
            return;
        }

        if(fork() == 0) {
            int in = 0;
            int fd[2];

            int i;
            for (i = 0; i < cmd.commandCount() - 1; i++) {
                pipe(fd);
                spawnProc(cmd.getCommand(i), in, fd[1]);
                close(fd[1]);
                in = fd[0];
            }

            if (in != 0)
                dup2(in, 0);

            execvp(
                    cmd.getCommand(i)[0],
                    cmd.getCommand(i)
            );
        }
    }

    void changeDirectory(const string& path) const {
        chdir(path.c_str());
    }

private:
    int spawnProc(char** cmd, int in, int out) {
        pid_t pid = fork();
        if(pid == 0) {
            if(in != 0) {
                dup2(in, 0);
                close(in);
            }
            if(out != 1) {
                dup2(out, 1);
                close(out);
            }

            return execvp(cmd[0], cmd);
        }

        return pid;
    }

};