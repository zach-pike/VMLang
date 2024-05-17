#include "vm/vm.hpp"
#include "compiler/compiler.hpp"

#include <assert.h>

int main() {
    Compiler cmplr;
    cmplr.parseFile("../src/test/programfiles/fib.vm");
    cmplr.compileAndWriteBinary("/tmp/fib.vmc");

    VM vm;
    vm.initializeVM();

    vm.getMemory().loadFromFile("/tmp/fib.vmc");

    while(!vm.stepExecution(false));

    VMValue v = vm.getStack().pop();

    assert((v.vartype == VMValueType::UINT));
    assert((v.size == 4));
    assert((v.value.uInt == 55));

    return 0;
}