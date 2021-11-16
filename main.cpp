#include <iostream>
#include "CommandExecutor.hpp"

using namespace std;

int main() {
    auto executor = new CommandExecutor();
    executor->execute("ls -lah|grep c|grep make");
}