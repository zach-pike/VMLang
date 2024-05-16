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
    // Iterate backwards thru list

    for (int i=stack.size()-1; i >= 0; i--) {
        VMValue itm = stack.at(i);

        // Print the item
        std::string s;
        s.append("Stack item ");

        switch (itm.vartype) {
            case VMValueType::UINT: {
                s.push_back('u');
                s.append(std::to_string(itm.size * 8));
                s.push_back('(');
                s.append(std::to_string(itm.value.uInt));
                s.push_back(')');
            } break;
            case VMValueType::SINT: {
                s.push_back('s');
                s.append(std::to_string(itm.size * 8));
                s.push_back('(');
                s.append(std::to_string(itm.value.sInt));
                s.push_back(')');
            } break;
            case VMValueType::DOUBLE: {
                s.append("double(");
                s.append(std::to_string(itm.value.doubleVal));
                s.push_back(')');
            } break;
            default: throw std::runtime_error("what the fuck");
        }

        std::cout << s << '\n';
    }
}