#ifndef INPUT_HPP
#define INPUT_HPP

#include <stdint.h>
#include <fstream>
#include <vector>

#include "Helpers.hpp"
#include "Blob.hpp"

class Input {
    private:
    uint256_t prevTransaction;
    uint32_t txoutIndex;
    uint64_t scriptLength;
    std::vector<uint8_t> scriptSig;
    uint32_t sequenceNumber;
    
    public:
    Input(std::istream & input);
    void init(std::istream & input);
    
    uint32_t getTxOutIndex() const;
    uint32_t getSequenceNumber() const;
    
    uint256_t getPreviousTransaction() const;
};

#endif
