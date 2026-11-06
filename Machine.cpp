#include "Machine.h"

Machine::Machine() : state(READY) {}

void Machine::loadProgram(const std::vector<int>& program, int startAddress) {
    for (size_t i = 0; i < program.size(); ++i) {
        int instrVal = program[i];
        int highByte = (instrVal >> 8) & 0xFF;
        int lowByte = instrVal & 0xFF;
        memory.write(startAddress + (i * 2), highByte);
        memory.write(startAddress + (i * 2) + 1, lowByte);
    }
}

void Machine::run() {
    state = RUNNING;
    while (state == RUNNING) {
        int currentPC = pc.get();
        int highByte = memory.read(currentPC);
        int lowByte = memory.read(currentPC + 1);

        if (highByte == 0 && lowByte == 0) {
            break;
        }

        int rawInstruction = (highByte << 8) | lowByte;
        std::stringstream ss;
        ss << std::hex << std::setw(4) << std::setfill('0') << rawInstruction;

        Instruction instr = decoder.decode(ss.str());
        executor.execute(instr, memory, regFile, pc, state);
    }
}

void Machine::dumpState() const {
    std::cout << "\n     ================ CPU REGISTERS ================\n";
    std::cout << "PC: " << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << pc.get() << "\n";
    for (int i = 0; i < 16; ++i) {
        std::cout << "R[" << std::uppercase << std::hex << i << "] = "
                  << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << regFile.getIndex(i) << "\t";
        if ((i + 1) % 4 == 0) std::cout << "\n";
    }

    std::cout << "\n\n          ================ MAIN MEMORY (16x16 MATRIX) ================\n\n";
    std::cout << "    ";
    for (int c = 0; c < 16; ++c) {
        std::cout << std::uppercase << std::hex << c << "    ";
    }
    std::cout << "\n----------------------------------------------------------------------------------\n";

    for (int r = 0; r < 16; ++r) {
        std::cout << std::uppercase << std::hex << r << " | ";
        for (int c = 0; c < 16; ++c) {
            int addr = (r * 16) + c;
            std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << memory.read(addr) << "   ";
        }
        std::cout << "\n\n";
    }
    std::cout << "==================================================================================\n";
}