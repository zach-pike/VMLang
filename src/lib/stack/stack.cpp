#include "stack.hpp"

#include <string>
#include <iostream>

Stack::Stack() {}
Stack::~Stack() {}

void Stack::pushStackItem(StackItem item) {
    stack.push_back(item);
}

StackItem Stack::popStackItem() {
    StackItem a = peek();
    stack.pop_back();
    return a;
}

StackItem Stack::peek() const {
    return stack.back();
}

void Stack::dump() const {
    // Iterate backwards thru list

    for (int i=stack.size()-1; i >= 0; i--) {
        StackItem itm = stack.at(i);

        // Print the item
        std::string s;
        s.append("Stack item (");

        if (itm.isSigned) s.push_back('S'); else s.push_back('U');
        s.append(std::to_string(itm.size*8));
        s.append("): ");
        s.append(std::to_string(itm.value));

        std::cout << s << '\n';
    }
}