#include "vm/vm.hpp"
#include "compiler/compiler.hpp"

#include "testingmmio/testingmmio.hpp"

#include <assert.h>

int main() {
    Compiler cmplr;
    cmplr.parseFile("../src/test/testfiles/stack.vm");
    cmplr.compileAndWriteBinary("/tmp/stack.vmc");

    VM vm;
    vm.initializeVM();

    vm.getMemory().loadFromFile("/tmp/stack.vmc");
    vm.getMemory().addMMIODevice(std::make_shared<TestingMMIO>(100000));

    while(!vm.stepExecution(false));

    return 0;
}