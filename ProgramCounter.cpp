#include "ProgramCounter.h"

ProgramCounter::ProgramCounter(int startAddress) : pc(startAddress) {}

int ProgramCounter::get() const { return pc; }

void ProgramCounter::set(int address) { pc = address; }

void ProgramCounter::increment() { pc += 2; }

void ProgramCounter::reset() { pc = 0; }