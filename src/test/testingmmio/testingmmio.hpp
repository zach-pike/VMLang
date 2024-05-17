#include "vm/memory/mmio/mmio.hpp"

#include <cstdint>

class TestingMMIO : public MMIODevice {
private:
    MMIORange range;
public:
    TestingMMIO(std::uint64_t address);
    ~TestingMMIO();

    MMIORange getMMIORange();
    void onWrite(std::uint8_t value, std::uint64_t address);
    std::uint8_t onRead(std::uint64_t address);
};