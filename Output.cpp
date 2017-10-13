#include "Output.hpp"

Output::Output(std::istream & input) {
    this->init(input);
}

void Output::init(std::istream & input) {
    input.read((char *)(&this->value), 8);
        
    scriptLength = varint(input);
    
    for (uint32_t i = 0; i < scriptLength; i++) {
        uint8_t tmp;
        input.read((char *)(&tmp), 1);    
        this->script.push_back(tmp);
    }
}

uint64_t Output::getValue() const {
    return this->value;
}
