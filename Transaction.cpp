#include "Transaction.hpp"

Transaction::Transaction(std::istream & input) {
    this->init(input);
}

void Transaction::init(std::istream & input) {
    input.read((char *)(&(this->version)), 4);
    this->inputsCounter = varint(input);
    
    for (uint32_t i = 0; i < this->inputsCounter; i++) {
        inputs.push_back(Input(input));
    }
    
    this->outputsCounter = varint(input);
    
    for (uint32_t i = 0; i < this->outputsCounter; i++) {
        outputs.push_back(Output(input));
    }
    
    input.read((char *)(&(this->lockTime)), 4);
    
}

uint64_t Transaction::getOutputsValue() const {
    uint64_t sum = 0;
    
    for (const auto & output : this->outputs) {
        sum += output.getValue();
    }
    
    return sum;
}
