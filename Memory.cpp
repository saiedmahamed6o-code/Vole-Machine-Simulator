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