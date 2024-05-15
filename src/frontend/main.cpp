#include "lib/vm.hpp"
#include "lib/compiler/compiler.hpp"
#include "lib/errors/errors.hpp"

#include "lib/compiler/lexer/lexer.hpp"

#include "argparse/argparse.hpp"

#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

int main(int argc, char** argv) {
    argparse::ArgumentParser programArgs("vmcl");

    programArgs.add_argument("-r")
        .help("File to run in VM");
    programArgs.add_argument("--debugLog")
        .help("Print debug log statements")
        .default_value(false)
        .flag();
    programArgs.add_argument("-d")
        .help("Debug a program");
    programArgs.add_argument("-c")
        .help("Compile a program file")
        .nargs(argparse::nargs_pattern::at_least_one);
    programArgs.add_argument("-o")
        .help("Set a output file for compilation");

    try {
        programArgs.parse_args(argc, argv);
    } catch(std::exception& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << programArgs;
        exit(1);
    }

    bool printDebug = programArgs.get<bool>("--debugLog");

    if (programArgs.is_used("-r")) {
        VM vm;

        vm.initializeVM();

        // Get reference to mem and stack
        SystemMemory& mem = vm.getMemory();
        Stack&      stack = vm.getStack();

        mem.loadFromFile(programArgs.get("-r"));

        try {
            while(!vm.stepExecution(printDebug));
        } catch(ArgumentException e) {
            std::cout << "Error in execution! ArgumentException! what()->\"" << e.what() << "\"\n";
        }

    } else if (programArgs.is_used("-c")) {
        // Compile program
        auto files = programArgs.get<std::vector<std::string>>("-c");

        Compiler compiler;

        for (const auto& file : files) {
            compiler.parseFile(file);
        }

        compiler.compileAndWriteBinary(programArgs.get("-o"));
    } else if (programArgs.is_used("-d")) {
        VM vm;
        vm.initializeVM();          // Initialize memory and stack

        // Get reference to mem and stack
        SystemMemory& mem = vm.getMemory();
        Stack&      stack = vm.getStack();

        mem.loadFromFile(programArgs.get("-d"));

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
                case 'r': {
                    try {
                        while(!vm.stepExecution(true));
                    } catch(ArgumentException e) {
                        std::cout << "Error in execution! ArgumentException! what()->\"" << e.what() << "\"\n";
                    }
                } break;

                case 'q': exit(0);
            }
        }
    } else {
        std::cout << programArgs;
    }


    return 0;
}