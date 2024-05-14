#pragma once

#include <vector>
#include <cstdint>

struct StackItem {
    std::uint64_t value;
    std::uint8_t  size;
    bool          isSigned;
};

class Stack {
private:
    std::vector<StackItem> stack;
public:
    Stack();
    ~Stack();

    void pushStackItem(StackItem item);
    StackItem popStackItem();

    void dump() const;
};