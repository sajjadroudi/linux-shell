#pragma once

#include <unistd.h>
#include <fstream>
#include <sys/wait.h>
#include "Command.h"
#include "CommandExecutor.h"

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
            exit(0);
        }

        if(cmd.isFileExecutionCommand()) {
            char* filePath = cmd.getCommand(0)[1];
            executeFile(filePath);
            return;
        }

        if(cmd.isAliasCommand()) {
            handleAliasCommand(cmd);
            return;
        }

        if(cmd.isUnaliasCommand()) {
            char* alias = cmd.getCommand(0)[1];
            AliasHandler::getInstance()->removeAlias(alias);
            return;
        }

        executeCommand(cmd);
    }

    void changeDirectory(char* path) {
        struct passwd *pw = getpwuid(getuid());
        char* home = pw->pw_dir;

        if(path == NULL) {
            chdir(home);
            return;
        }

        path = replaceCharStar(path, "~", home);

        if(chdir(path) == -1) {
            cerr << path << ": no such directory" << endl;
        }
    }

    string getOutput(const Command& command) {
        // Only single commands are supported
        if(command.commandCount() != 1) {
            return string{};
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

    void handleAliasCommand(const Command& cmd) {
        string alias;
        char** command = cmd.getCommand(0);
        for(int i = 1; command[i] != NULL; i++) {
            alias.append(command[i]).append(" ");
        }
        alias = rtrim(alias);

        auto pieces = splitIntoTwoPieces(alias, "=");
        AliasHandler::getInstance()->saveAlias(pieces->at(0), pieces->at(1));
    }

    void executeCommand(const Command& cmd) {
        if(fork() == 0) {
            int in = 0;
            int fd[2];

            int i;
            for (i = 0; i < cmd.commandCount() - 1; i++) {
                pipe(fd);
                forkProcess(cmd.getCommand(i), in, fd[1]);
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

    void forkProcess(char** command, int in, int out) const {
        if(fork() == 0) {
            if(in != 0) {
                dup2(in, 0);
                close(in);
            }
            if(out != 1) {
                dup2(out, 1);
                close(out);
            }
            execvp(command[0], command);
        }
    }

};