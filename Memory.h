#ifndef MEMORY_H
#define MEMORY_H

#include <vector>

enum machineState {
    READY,
    RUNNING,
    HALTED
};

class Memory {
private:
    std::vector<int> cells;

public:
    Memory(int size = 256);
    int read(int address) const;
    void write(int address, int value);
};

#endif