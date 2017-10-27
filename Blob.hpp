#ifndef BLOB_HPP
#define BLOB_HPP

#include <stdint.h>

template <unsigned int BYTES>
class Blob {
    private:
    uint8_t data[BYTES];
    
    public:
    uint32_t getSize() { return BYTES; }
    uint8_t& operator[](uint32_t byte);
    
    uint8_t* first();
    
    std::string getHexString() const;
};

#include "Blob.cpp"

typedef Blob<32> uint256_t;

#endif
