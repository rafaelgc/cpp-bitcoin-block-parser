#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <stdint.h>
#include <fstream>
#include <vector>

#include "Helpers.hpp"

class Output {
    public:
    Output(std::istream & input);
    void init(std::istream & input);
    
    uint64_t getValue() const;
    
    private:
    uint64_t value;
    uint64_t scriptLength;
    std::vector<uint8_t> script;
};

#endif
