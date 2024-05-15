#include "lib/vm.hpp"
#include "lib/compiler/programAssembler/prgmAssembler.hpp"
#include "lib/errors/errors.hpp"

#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cassert>

int main() {
    ProgramAssembler compiler;

    compiler.setInsertOffset(0x100);

    compiler.insertInstruction(Instructions::Push, InstructionArg(InstructionArgType::IMM, 8, 2000, false));
    compiler.insertInstruction(Instructions::Push, InstructionArg(InstructionArgType::IMM, 8, 2000, false));
    compiler.insertInstruction(Instructions::AddStack);
    compiler.insertInstruction(Instructions::Halt);

    VM vm;
    vm.initializeVM();          // Initialize memory and stack

    // Get reference to mem and stack
    SystemMemory& mem = vm.getMemory();
    Stack&      stack = vm.getStack();

    // Copy program to system ram
    memcpy(mem.getRaw(), compiler.getRaw(), VM_MEMORY_SIZE);

    for (int i=0; i<4; i++) {
        try {
            vm.stepExecution(false);
        } catch(ArgumentException e) {
            std::cout << "Error in execution! ArgumentException! what()->\"" << e.what() << "\"\n";
        }
    }

    StackItem item = stack.popStackItem();

    return item.value != 4000;
}