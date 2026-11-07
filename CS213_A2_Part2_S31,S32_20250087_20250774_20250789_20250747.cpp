#include <iostream>
#include <vector>
#include "Machine.h"

int main() {
    Machine myMachine;


    std::vector<int> userProgram;
    std::string hexInput;
    std::cout << "Enter instructions in Hex (e.g., 2005, C000 for HALT):\n";
    while (std::cin >> hexInput) {
        if (hexInput == "C000") {
            userProgram.push_back(0xC000);
            break;
        }
        std::stringstream ss;
        ss << std::hex << hexInput;
        int val;
        ss >> val;
        userProgram.push_back(val);
    }

    myMachine.loadProgram(userProgram, 0);
    myMachine.run();
    myMachine.dumpState();

    return 0;
}