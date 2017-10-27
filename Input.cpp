#include "Input.hpp"

Input::Input(std::istream & input) {
    this->init(input);
}

void Input::init(std::istream & input) {
    //input.read((char *)(&this->prevTransaction), 32);  
    input.read((char *) this->prevTransaction.first(), 32);
    input.read((char *)(&this->txoutIndex), 4);    
    this->scriptLength = varint(input);
    
    for (uint32_t i = 0; i < scriptLength; i++) {
        uint8_t tmp;
        input.read((char *)(&tmp), 1);
        this->scriptSig.push_back(tmp);
    }
    
    input.read((char *)(&this->sequenceNumber), 4);    
}

uint32_t Input::getSequenceNumber() const {
    return this->sequenceNumber;
}

uint32_t Input::getTxOutIndex() const {
    return this->txoutIndex;
}

uint256_t Input::getPreviousTransaction() const {
    return prevTransaction;
}
