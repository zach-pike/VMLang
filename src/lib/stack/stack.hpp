#pragma once

#include <vector>
#include <cstdint>

#include "lib/variable/variable.hpp"

class Stack {
private:
    std::vector<VMVariableDatatype> stack;
public:
    Stack();
    ~Stack();

    void push(VMVariableDatatype item);
    VMVariableDatatype pop();
    VMVariableDatatype peek() const;

    void dump() const;
};