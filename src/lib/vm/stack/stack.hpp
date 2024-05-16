#pragma once

#include <vector>
#include <cstdint>

#include "vm/value/value.hpp"

class Stack {
private:
    std::vector<VMValue> stack;
public:
    Stack();
    ~Stack();

    void push(VMValue item);
    VMValue pop();
    VMValue peek() const;

    void dump() const;
};