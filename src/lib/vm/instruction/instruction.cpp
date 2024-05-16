#include "instruction.hpp"

#include <sstream>
#include "vm/registers/registers.hpp"

InstructionArg::InstructionArg(VMValue _var, InstructionArgType _type):
    var(_var),
    type(_type) {}

std::string InstructionArg::toString() const {
    std::string s;

    s.append("Arg { ");

    switch(type) {
        case InstructionArgType::NUMBER: {
            std::stringstream ss;
            ss << var;
            s.append(ss.str());
        } break;
        case InstructionArgType::REGISTER: {
            s.append("register(%");
            s.append(getRegNames()[var.value.uInt]);
            s.push_back(')');
        } break;
    }

    s.append(" }");

    return s;
}