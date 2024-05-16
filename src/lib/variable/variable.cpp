#include "variable.hpp"

VMVariableDatatype::VMVariableDatatype():
    isUninitialized(true) {}

VMVariableDatatype::VMVariableDatatype(std::uint8_t v):
    vartype(VMVariableType::UINT),
    size(1),
    value(ValueUnion{ .uInt = v }) {}

VMVariableDatatype::VMVariableDatatype(std::int8_t v):
    vartype(VMVariableType::SINT),
    size(1),
    value(ValueUnion{ .sInt = v }) {}

VMVariableDatatype::VMVariableDatatype(std::uint16_t v):
    vartype(VMVariableType::UINT),
    size(2),
    value(ValueUnion{ .uInt = v }) {}

VMVariableDatatype::VMVariableDatatype(std::int16_t v):
    vartype(VMVariableType::SINT),
    size(2),
    value(ValueUnion{ .sInt = v }) {}

VMVariableDatatype::VMVariableDatatype(std::uint32_t v):
    vartype(VMVariableType::UINT),
    size(4),
    value(ValueUnion{ .uInt = v }) {}

VMVariableDatatype::VMVariableDatatype(std::int32_t v):
    vartype(VMVariableType::SINT),
    size(4),
    value(ValueUnion{ .sInt = v }) {}

VMVariableDatatype::VMVariableDatatype(std::uint64_t v):
    vartype(VMVariableType::UINT),
    size(8),
    value(ValueUnion{ .uInt = v }) {}

VMVariableDatatype::VMVariableDatatype(std::int64_t v):
    vartype(VMVariableType::SINT),
    size(8),
    value(ValueUnion{ .sInt = v }) {}

VMVariableDatatype::VMVariableDatatype(float v):
    vartype(VMVariableType::FLOAT),
    size(sizeof(v)),
    value(ValueUnion{ .floatVal = v }) {}

VMVariableDatatype::VMVariableDatatype(double v):
    vartype(VMVariableType::DOUBLE),
    size(sizeof(double)),
    value(ValueUnion{ .doubleVal = v }) {}


VMVariableDatatype VMVariableDatatype::operator+(const VMVariableDatatype& rhs) {
    // Add logic

    if ((vartype == VMVariableType::SINT || vartype == VMVariableType::UINT)
     && (rhs.vartype == VMVariableType::SINT || rhs.vartype == VMVariableType::UINT)) {

        bool resultSigned = (vartype == VMVariableType::SINT) || (rhs.vartype == VMVariableType::SINT);

        if (resultSigned) {
            // Try to read both values as signed value
            std::int64_t v1;
            if (rhs.vartype == VMVariableType::UINT)
                v1 = value.uInt; // Cast to signed num
        }
    }
}

VMVariableDatatype VMVariableDatatype::operator-(const VMVariableDatatype& rhs) {

}

std::ostream& operator<<(std::ostream &out, VMVariableDatatype const& a) {
    out << "VMVar { ";

    if (a.isUninitialized) {
        out << "Null }";
        return out;
    }

    switch(a.vartype) {
        case VMVariableType::UINT:   out << 'u' << (int)a.size*8 << '(' << a.value.uInt; break;
        case VMVariableType::SINT:   out << 's' << (int)a.size*8 << '(' << a.value.sInt; break;
        case VMVariableType::FLOAT:  out << "float" << '(' << a.value.floatVal;          break;
        case VMVariableType::DOUBLE: out << "double" << '(' << a.value.doubleVal;        break;
    }

    out << ") }";

    return out;
}