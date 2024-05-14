#include "lib/vm.hpp"
#include "lib/compiler/compiler.hpp"

#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main() {
    BuiltinCompiler compiler;

    compiler.setInsertOffset(0x100);

    InstructionArg a;
    a.type = InstructionArgType::IMM;
    a.size = 4;
    a.isSigned = false;
    a.value = 69420;

    compiler.insertInstruction(Instructions::Halt, a);

    VM vm;
    vm.initializeVM();

    SystemMemory& mem = vm.getMemory();

    // Copy program to system ram
    memcpy(mem.getRaw(), compiler.getRaw(), VM_MEMORY_SIZE);

    while(true) {
        printf("> ");
        char c;
        std::cin >> c;

        switch(c) {
            case 's': {
                vm.stepExecution(true);
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