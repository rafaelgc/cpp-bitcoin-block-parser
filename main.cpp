#include <iostream>
#include <vector>
#include "picosha2.h"

#include "Helpers.hpp"
#include "Input.hpp"
#include "Output.hpp"
#include "Transaction.hpp"
#include "BlockHeader.hpp"
#include "Block.hpp"


int main(int argc, char ** argv) {
    if (argc < 2) {
        std::cout << "Command: " << argv[0] << " FILENAME" << std::endl;
    }
    

    std::ifstream input(argv[1], std::ios::binary);
    
    uint32_t counter = 0;
    
    while (!input.eof()) {
        Block block(input);
        
        if (!input.fail()) {
            std::cout << block.getHeader().getHash() << std::endl;
            std::cout << std::dec << "Size: " << block.getSize() << std::endl;
            std::cout << "Output Satoshi: " << std::dec << block.getOutputsValue() << " in " << block.getTransactionCounter() << " transactions. " << std::endl;
            std::cout << "MR: " << std::dec << *block.getHeader().getMerkleRoot() << std::endl;
            std::cout << input.tellg() << std::endl << std::endl;
            counter++;
        }
    }
    
    std::cout << "Blocks: " << std::dec << counter << std::endl;
    
    input.close();
    
    return 0;
}

