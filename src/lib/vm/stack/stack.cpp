#include "stack.hpp"

#include <string>
#include <iostream>

Stack::Stack() {}
Stack::~Stack() {}

void Stack::push(VMValue item) {
    stack.push_back(item);
}

VMValue Stack::pop() {
    VMValue a = peek();
    stack.pop_back();
    return a;
}

VMValue Stack::peek() const {
    return stack.back();
}

void Stack::dump() const {
    for (const VMValue& itm : stack) {
        // Print the item
        std::cout << "Stack item: " << itm << '\n';
    }
}