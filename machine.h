#ifndef MACHINE_H
#define MACHINE_H

#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include "Memory.h"
#include "RegisterFile.h"
#include "ProgramCounter.h"
#include "Instruction.h"
#include "InstructionExecutor.h"

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

#endif