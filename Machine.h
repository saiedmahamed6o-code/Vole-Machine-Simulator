#pragma once
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "Memory.h"
#include "CPUComponents.h"
#include "Instruction.h"

class Machine {
private:
    Memory memory;
    RegisterFile regFile;
    ProgramCounter pc;
    Instruction decoder;
    InstructionExecutor executor;
    machineState state;

public:
    Machine();
    void loadProgram(const std::vector<int>& program, int startAddress = 0);
    void run();
    void dumpState() const;
};