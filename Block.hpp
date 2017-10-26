#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <stdint.h>
#include <fstream>
#include <vector>

#include "Helpers.hpp"
#include "BlockHeader.hpp"
#include "Transaction.hpp"

class Block {
    private:
    uint32_t magic, blockSize;
    BlockHeader blockHeader;
    uint64_t txnCounter;
    std::vector<Transaction> txs;
    public:
    Block(std::istream & input);
    
    uint32_t getMagicNumber() const;
    uint32_t getSize() const;
    BlockHeader getHeader() const;
    uint32_t getTransactionCounter() const;
    
    uint64_t getOutputsValue() const;
    
    std::vector<uint8_t> calcMerkleRoot();
};

#endif
