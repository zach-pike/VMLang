#include "lib/memory/memory.hpp"

#include "string.h"

int main() {
    char data[64] = { 0 };

    memcpy(data, "hello world!", 12);

    memDump((std::uint8_t*)data, 0, 64);
}