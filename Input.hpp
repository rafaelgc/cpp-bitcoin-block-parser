#ifndef INPUT_HPP
#define INPUT_HPP

#include <stdint.h>
#include <fstream>
#include <vector>

#include "Helpers.hpp"

class Input {
    private:
    uint8_t prevTransaction[32];
    uint32_t txoutIndex;
    uint64_t scriptLength;
    std::vector<uint8_t> scriptSig;
    uint32_t sequenceNumber;
    
    public:
    Input(std::istream & input);
    void init(std::istream & input);
    
    uint32_t getSequenceNumber() const;
};

#endif
