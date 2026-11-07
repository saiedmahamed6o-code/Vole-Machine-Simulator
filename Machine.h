#pragma once
#include <vector>
#include <string>
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

class Instruction {
    private:
        int opCode;
        int R;
        int v1;
        int v2;
        int hexaCharToDecimal(char hexChar);

    public:
        Instruction();
        Instruction(int opcode, int R, int v1, int v2);
        Instruction decode(std::string instructionCode);
        int getOpcode() const;
        int getOperand1() const;
        int getOperand2() const;
        int getOperand3() const;
};
class InstructionExecutor {
    public:
        void execute(const Instruction& instr, Memory& memory, RegisterFile& regFile, ProgramCounter& pc, machineState& state);
};
#pragma once
#include <vector>

class Memory {
private:
    std::vector<int> cells;

public:
    Memory(int size = 256);
    int read(int address) const;
    void write(int address, int value);
};