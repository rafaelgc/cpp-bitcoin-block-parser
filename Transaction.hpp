#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <stdint.h>
#include <fstream>
#include <vector>

#include "Helpers.hpp"
#include "Input.hpp"
#include "Output.hpp"

class Transaction {
    private:
    uint32_t version;
    uint64_t inputsCounter;
    std::vector<Input> inputs;
    uint64_t outputsCounter;
    std::vector<Output> outputs;
    uint32_t lockTime;
    
    public:
    Transaction(std::istream & input);
    void init(std::istream & input);
    
    uint64_t getOutputsValue() const;
};

#endif
