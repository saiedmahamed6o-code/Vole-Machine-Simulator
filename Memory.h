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