template <unsigned int BYTES>
uint8_t& Blob<BYTES>::operator[](uint32_t byte) {
    return data[byte];
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
