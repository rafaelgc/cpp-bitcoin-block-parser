#ifndef BLOB_HPP
#define BLOB_HPP

#include <stdint.h>
#include "Helpers.hpp"

template <unsigned int BYTES>
class Blob {
    public:
    Blob();
    uint32_t getSize() { return BYTES; }
    uint8_t& operator[](uint32_t byte);
    Blob& operator<<(uint8_t byte);
    
    uint8_t* first();
    
    std::string getHexString() const;
    
    private:
    uint8_t data[BYTES];
};

#include "Blob.cpp"

typedef Blob<32> uint256_t;

#endif
