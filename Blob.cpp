
#include "Blob.hpp"

template <unsigned int BYTES>
Blob<BYTES>::Blob() {
    for (unsigned int i = 0; i < BYTES; i++) {
        data[i] = 0;
    }
}

template <unsigned int BYTES>
uint8_t& Blob<BYTES>::operator[](uint32_t byte) {
    return data[byte];
}

template<unsigned int BYTES>
Blob<BYTES>& Blob<BYTES>::operator <<(uint8_t byte) {
    for (unsigned int i = 1; i < BYTES; i++) {
        data[i - 1] = data[i];
    }
    
    data[BYTES - 1] = byte;
}

template <unsigned int BYTES>
uint8_t* Blob<BYTES>::first() {
    return data;
}

template <unsigned int BYTES>
std::string Blob<BYTES>::getHexString() const {
    std::string result;
    
    for (uint32_t i = 0; i < BYTES; i++) {
        result += uint8ToHexTop(data[i]);
        result += uint8ToHexBot(data[i]);
    }
    
    return result;
}
