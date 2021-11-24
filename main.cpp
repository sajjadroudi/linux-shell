#include "Shell.hpp"

using namespace std;

int main() {
    auto shell = Shell::getInstance();
    shell->run();
}