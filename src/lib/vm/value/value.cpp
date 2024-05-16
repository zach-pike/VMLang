#include "value.hpp"

VMValue::VMValue():
    isUninitialized(true) {}

VMValue::VMValue(std::uint8_t v):
    vartype(VMValueType::UINT),
    size(1),
    value(ValueUnion{ .uInt = v }) {}

VMValue::VMValue(std::int8_t v):
    vartype(VMValueType::SINT),
    size(1),
    value(ValueUnion{ .sInt = v }) {}

VMValue::VMValue(std::uint16_t v):
    vartype(VMValueType::UINT),
    size(2),
    value(ValueUnion{ .uInt = v }) {}

VMValue::VMValue(std::int16_t v):
    vartype(VMValueType::SINT),
    size(2),
    value(ValueUnion{ .sInt = v }) {}

VMValue::VMValue(std::uint32_t v):
    vartype(VMValueType::UINT),
    size(4),
    value(ValueUnion{ .uInt = v }) {}

VMValue::VMValue(std::int32_t v):
    vartype(VMValueType::SINT),
    size(4),
    value(ValueUnion{ .sInt = v }) {}

VMValue::VMValue(std::uint64_t v):
    vartype(VMValueType::UINT),
    size(8),
    value(ValueUnion{ .uInt = v }) {}

VMValue::VMValue(std::int64_t v):
    vartype(VMValueType::SINT),
    size(8),
    value(ValueUnion{ .sInt = v }) {}

VMValue::VMValue(double v):
    vartype(VMValueType::DOUBLE),
    size(sizeof(double)),
    value(ValueUnion{ .doubleVal = v }) {}

VMValue VMValue::operator+(const VMValue& rhs) {
    if ((vartype == VMValueType::SINT || vartype == VMValueType::UINT)
     && (rhs.vartype == VMValueType::SINT || rhs.vartype == VMValueType::UINT)) {

        bool resultSigned = (vartype == VMValueType::SINT) || (rhs.vartype == VMValueType::SINT);

        if (resultSigned) {
            // Try to read both values as signed value
            std::int64_t v1;
            std::int64_t v2;

            if (vartype == VMValueType::UINT)
                v1 = value.uInt; // Cast to signed number
            else
                v1 = value.sInt;

            if (rhs.vartype == VMValueType::UINT)
                v2 = rhs.value.uInt;
            else
                v2 = rhs.value.sInt;

            std::int64_t sum = v1 + v2;

            auto v = VMValue();
            v.isUninitialized = false;
            v.value.sInt = sum;
            v.size = std::max(size, rhs.size);
            v.vartype = VMValueType::SINT;
            return v;
        } else {
            std::uint64_t sum = value.uInt + rhs.value.uInt;

            auto v = VMValue();
            v.isUninitialized = false;
            v.value.uInt = sum;
            v.size = std::max(size, rhs.size);
            v.vartype = VMValueType::UINT;
            return v;
        }
    } else {
        double result = 0;

        switch(vartype) {
            case VMValueType::UINT: result += (double)value.uInt; break;
            case VMValueType::SINT: result += (double)value.sInt; break;
            case VMValueType::DOUBLE: result += value.doubleVal; break;
        }

        switch(rhs.vartype) {
            case VMValueType::UINT: result += (double)rhs.value.uInt; break;
            case VMValueType::SINT: result += (double)rhs.value.sInt; break;
            case VMValueType::DOUBLE: result += rhs.value.doubleVal; break;
        }

        return VMValue(result);
    }
}

VMValue VMValue::operator-(const VMValue& rhs) {
    if ((vartype == VMValueType::SINT || vartype == VMValueType::UINT)
     && (rhs.vartype == VMValueType::SINT || rhs.vartype == VMValueType::UINT)) {

        bool resultSigned = (vartype == VMValueType::SINT) || (rhs.vartype == VMValueType::SINT);

        if (resultSigned) {
            // Try to read both values as signed value
            std::int64_t v1;
            std::int64_t v2;

            if (vartype == VMValueType::UINT)
                v1 = value.uInt; // Cast to signed number
            else
                v1 = value.sInt;

            if (rhs.vartype == VMValueType::UINT)
                v2 = rhs.value.uInt;
            else
                v2 = rhs.value.sInt;

            std::int64_t diff = v1 - v2;

            auto v = VMValue();
            v.isUninitialized = false;
            v.value.sInt = diff;
            v.size = std::max(size, rhs.size);
            v.vartype = VMValueType::SINT;
            return v;
        } else {
            std::uint64_t diff = value.uInt - rhs.value.uInt;

            auto v = VMValue();
            v.isUninitialized = false;
            v.value.uInt = diff;
            v.size = std::max(size, rhs.size);
            v.vartype = VMValueType::UINT;
            return v;
        }
    } else {
        double result = 0;

        switch(vartype) {
            case VMValueType::UINT: result = (double)value.uInt; break;
            case VMValueType::SINT: result = (double)value.sInt; break;
            case VMValueType::DOUBLE: result = value.doubleVal; break;
        }

        switch(rhs.vartype) {
            case VMValueType::UINT: result -= (double)rhs.value.uInt; break;
            case VMValueType::SINT: result -= (double)rhs.value.sInt; break;
            case VMValueType::DOUBLE: result -= rhs.value.doubleVal; break;
        }

        return VMValue(result);
    }
}

std::ostream& operator<<(std::ostream &out, VMValue const& a) {
    out << "VMVar { ";

    if (a.isUninitialized) {
        out << "Null }";
        return out;
    }

    switch(a.vartype) {
        case VMValueType::UINT:   out << 'u' << (int)a.size*8 << '(' << std::to_string(a.value.uInt); break;
        case VMValueType::SINT:   out << 's' << (int)a.size*8 << '(' << std::to_string(a.value.sInt); break;
        case VMValueType::DOUBLE: out << "double" << '(' << a.value.doubleVal;        break;
    }

    out << ") }";

    return out;
}