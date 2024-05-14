#include "lib/vm.hpp"
#include "lib/compiler/compiler.hpp"
#include "lib/errors/errors.hpp"

#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main() {
    BuiltinCompiler compiler;

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

    while(true) {
        printf("> ");

        char c;
        std::cin >> c;

        switch(c) {
            case 's': {
                try {
                    vm.stepExecution(true);
                } catch(ArgumentException e) {
                    std::cout << "Error in execution! ArgumentException! what()->\"" << e.what() << "\"\n";
                }
            } break;
            case 'd': {
                // Dump registers and stack
                vm.dumpEverything();
            } break;

            case 'q': exit(0);
        }
    }

    return 0;
}