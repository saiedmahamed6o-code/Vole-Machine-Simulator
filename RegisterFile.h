#ifndef REGISTER_FILE_H
#define REGISTER_FILE_H

#include <vector>

class RegisterFile {
private:
    std::vector<int> registers;

public:
    RegisterFile(int size = 16);
    int getIndex(int index) const;
    void setIndex(int index, int value);
};

#endif