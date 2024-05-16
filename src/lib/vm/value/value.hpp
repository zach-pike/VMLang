#pragma once

#include <cstdint>

#include <ostream>
#include <iostream>

enum class VMValueType : std::uint8_t {
    UINT,
    SINT,
    DOUBLE
};

struct VMValue {
    VMValueType vartype;
    std::uint8_t   size;

    bool isUninitialized = false;

    union ValueUnion {
        std::uint64_t uInt;
        std::int64_t  sInt;

        double        doubleVal;
    } value;

    VMValue();

    VMValue(std::uint8_t);
    VMValue(std::int8_t);

    VMValue(std::uint16_t);
    VMValue(std::int16_t);

    VMValue(std::uint32_t);
    VMValue(std::int32_t);

    VMValue(std::uint64_t);
    VMValue(std::int64_t);

    VMValue(double);

    VMValue operator+(const VMValue& rhs);
    VMValue operator-(const VMValue& rhs);

    friend std::ostream& operator<<(std::ostream& os, VMValue const& obj);
} __attribute__((packed));

