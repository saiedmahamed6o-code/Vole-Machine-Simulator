#include "Machine.h"
#include "Instruction.h"
#include "Memory.h"
#include "CPUComponents.h"

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
int Instruction::hexaCharToDecimal(char hexChar) {
    if (hexChar >= '0' && hexChar <= '9') return hexChar - '0';
    if (hexChar >= 'A' && hexChar <= 'F') return 10 + (hexChar - 'A');
    if (hexChar >= 'a' && hexChar <= 'f') return 10 + (hexChar - 'a');
    return 0;
}

Instruction::Instruction() : opCode(0), R(0), v1(0), v2(0) {}
Instruction::Instruction(int opcode, int R, int v1, int v2) : opCode(opcode), R(R), v1(v1), v2(v2) {}

Instruction Instruction::decode(std::string instructionCode) {
    if (instructionCode.length() != 4) return Instruction(0, 0, 0, 0);
    int opcode = hexaCharToDecimal(instructionCode[0]);
    int R = hexaCharToDecimal(instructionCode[1]);
    int v1 = hexaCharToDecimal(instructionCode[2]);
    int v2 = hexaCharToDecimal(instructionCode[3]);
    return Instruction(opcode, R, v1, v2);
}

int Instruction::getOpcode() const { return opCode; }
int Instruction::getOperand1() const { return R; }
int getOperand2() const { return v1; } // (تأكد من إرجاع v1 داخل السكوب)
int Instruction::getOperand2() const { return v1; }
int Instruction::getOperand3() const { return v2; }

void InstructionExecutor::execute(const Instruction& instr, Memory& memory, RegisterFile& regFile, ProgramCounter& pc, machineState& state) {
    int opcode = instr.getOpcode();
    int op1 = instr.getOperand1();
    int op2 = instr.getOperand2();
    int op3 = instr.getOperand3();
    int address = (op2 << 4) | op3;

    switch (opcode) {
        case 0:
        case 12:
            state = HALTED;
            break;
        case 1:
            regFile.setIndex(op1, memory.read(address));
            break;
        case 2:
            regFile.setIndex(op1, address);
            break;
        case 3:
            memory.write(address, regFile.getIndex(op1));
            break;
        case 4: {
            int val = regFile.getIndex(op2);
            regFile.setIndex(op3, val);
            break;
        }
        case 5:
        case 6: {
            int valS = regFile.getIndex(op2);
            int valT = regFile.getIndex(op3);
            regFile.setIndex(op1, valS + valT);
            break;
        }
        case 7: {
            int valS = regFile.getIndex(op2);
            int valT = regFile.getIndex(op3);
            regFile.setIndex(op1, valS | valT);
            break;
        }
        case 8: {
            int valS = regFile.getIndex(op2);
            int valT = regFile.getIndex(op3);
            regFile.setIndex(op1, valS & valT);
            break;
        }
        case 9: {
            int valS = regFile.getIndex(op2);
            int valT = regFile.getIndex(op3);
            regFile.setIndex(op1, valS ^ valT);
            break;
        }
        case 10: {
            int val = regFile.getIndex(op1);
            int count = op3;
            int rotated = (val >> count) | (val << (8 - count));
            regFile.setIndex(op1, rotated & 255);
            break;
        }
        case 11: {
            if (regFile.getIndex(op1) == regFile.getIndex(0)) {
                pc.set(address);
                return;
            }
            break;
        }
        default:
            state = HALTED;
            break;
    }
    pc.increment();
}

#include "Memory.h"

Memory::Memory(int size) {
    cells.resize(size, 0);
}

int Memory::read(int address) const {
    if (address >= 0 && address < (int)cells.size()) {
        return cells[address];
    }
    return 0;
}

void Memory::write(int address, int value) {
    if (address >= 0 && address < (int)cells.size()) {
        cells[address] = value;
    }
}



RegisterFile::RegisterFile(int size) {
    registers.resize(size, 0);
}

int RegisterFile::getIndex(int index) const {
    if (index >= 0 && index < (int)registers.size()) {
        return registers[index];
    }
    return 0;
}

void RegisterFile::setIndex(int index, int value) {
    if (index >= 0 && index < (int)registers.size()) {
        registers[index] = value;
    }
}

ProgramCounter::ProgramCounter(int startAddress) : pc(startAddress) {}
int ProgramCounter::get() const { return pc; }
void ProgramCounter::set(int address) { pc = address; }
void ProgramCounter::increment() { pc += 2; }
void ProgramCounter::reset() { pc = 0; }