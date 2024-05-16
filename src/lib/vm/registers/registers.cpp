#include "registers.hpp"

#include "vm/errors/errors.hpp"

VMValue VMRegs::getReg(VMReg reg) const {
    switch(reg) {
        case VMReg::A: return a;
        case VMReg::B: return b;
        case VMReg::C: return c;
        case VMReg::D: return d;
        case VMReg::IP: return VMValue(ip);
        default: throw ArgumentException(0);
    }
}

void VMRegs::setReg(VMReg reg, VMValue var) {
    switch(reg) {
        case VMReg::A: a = var; break;
        case VMReg::B: b = var; break;
        case VMReg::C: c = var; break;
        case VMReg::D: d = var; break;
        case VMReg::IP: {
            if (var.vartype != VMValueType::UINT)
                throw ArgumentException(0);
            ip = var.value.uInt;
        } break;
    }
}