#pragma once

#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include "Command.hpp"
#include "CommandExecutor.hpp"

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

        if(cmd.isExitCommand()) {
            exit(1);
        }

        if(cmd.isFileExecutionCommand()) {
            char* path = cmd.getCommand(0)[1];
            executeFile(path);
            return;
        }

        if(cmd.isAliasCommand()) {
            string alias;
            char** command = cmd.getCommand(0);
            for(int i = 1; command[i] != NULL; i++) {
                alias.append(command[i]).append(" ");
            }
            alias = rtrim(alias);

            auto pieces = splitIntoTwoPieces(alias, "=");
            AliasHandler::getInstance()->saveAlias(pieces->at(0), pieces->at(1));
            return;
        }

        if(cmd.isUnaliasCommand()) {
            char* alias = cmd.getCommand(0)[1];
            AliasHandler::getInstance()->removeAlias(alias);
            return;
        }

        pid_t pid = fork();
        if(pid == 0) {
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
        } else {
            wait(NULL);
        }
    }

    void changeDirectory(const string& path) const {
        chdir(path.c_str());
    }

    string getOutput(const Command& command) {
        if(command.commandCount() != 1) {
            throw runtime_error("illegal argument");
        }

        string stringCommand = command.getStringCommand(0);
        char* cmd = new char[stringCommand.size()];
        strcpy(cmd, stringCommand.c_str());
        unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

        if (!pipe)
            throw runtime_error("popen() failed!");

        array<char, 128> buffer{};
        string result;
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
            result += buffer.data();

        return string(result.begin(), result.end() - 1);
    }

private:
    int spawnProc(char** cmd, int in, int out) const {
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

    void executeFile(const string& path) {
        ifstream file{path};

        if(!file) {
            cerr << "Something went wrong." << endl;
            return;
        }

        for(string line; getline(file, line); ) {
            execute(line);
        }

        file.close();
    }

};