#ifndef PROGRAM_COUNTER_H
#define PROGRAM_COUNTER_H

class ProgramCounter {
private:
    int pc;

public:
    ProgramCounter(int startAddress = 0);
    int get() const;
    void set(int address);
    void increment();
    void reset();
};

#endif