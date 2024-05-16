#include "stack.hpp"

#include <string>
#include <iostream>

Stack::Stack() {}
Stack::~Stack() {}

void Stack::push(VMVariableDatatype item) {
    stack.push_back(item);
}

VMVariableDatatype Stack::pop() {
    VMVariableDatatype a = peek();
    stack.pop_back();
    return a;
}

VMVariableDatatype Stack::peek() const {
    return stack.back();
}

void Stack::dump() const {
    // Iterate backwards thru list

    for (int i=stack.size()-1; i >= 0; i--) {
        VMVariableDatatype itm = stack.at(i);

        // Print the item
        std::string s;
        s.append("Stack item (");

        switch (itm.vartype) {
            case VMVariableType::UINT: {
                s.push_back('U');
                s.append(std::to_string(itm.size));
                s.append("): ");
                s.append(std::to_string(itm.value.uInt));
            } break;
            case VMVariableType::SINT: {
                s.push_back('S');
                s.append(std::to_string(itm.size));
                s.append("): ");
                s.append(std::to_string(itm.value.sInt));
            } break;
            case VMVariableType::FLOAT: {
                s.append("Float): ");
                s.append(std::to_string(itm.value.floatVal));
            } break;
            case VMVariableType::DOUBLE: {
                s.append("Float): ");
                s.append(std::to_string(itm.value.doubleVal));
            } break;
        }

        std::cout << s << '\n';
    }
}