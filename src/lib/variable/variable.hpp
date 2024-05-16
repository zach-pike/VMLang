#pragma once

#include <cstdint>

#include <ostream>
#include <iostream>

enum class VMVariableType : std::uint8_t {
    UINT,
    SINT,
    DOUBLE
};

struct VMVariableDatatype {
    VMVariableType vartype;
    std::uint8_t   size;

    bool isUninitialized = false;

    union ValueUnion {
        std::uint64_t uInt;
        std::int64_t  sInt;

        double        doubleVal;
    } value;

    VMVariableDatatype();

    VMVariableDatatype(std::uint8_t);
    VMVariableDatatype(std::int8_t);

    VMVariableDatatype(std::uint16_t);
    VMVariableDatatype(std::int16_t);

    VMVariableDatatype(std::uint32_t);
    VMVariableDatatype(std::int32_t);

    VMVariableDatatype(std::uint64_t);
    VMVariableDatatype(std::int64_t);

    VMVariableDatatype(double);

    VMVariableDatatype operator+(const VMVariableDatatype& rhs);
    VMVariableDatatype operator-(const VMVariableDatatype& rhs);

    friend std::ostream& operator<<(std::ostream& os, VMVariableDatatype const& obj);
} __attribute__((packed));

