#include "Block.hpp"

Block::Block(std::istream & input) {
    input.read((char *)(&magic), 4);
    input.read((char *)(&blockSize), 4);
    
    this->blockHeader.init(input);
    
    this->txnCounter = varint(input);
    
    for (uint32_t i = 0; i < txnCounter; i++) {
        txs.push_back(Transaction(input));
    }
}

uint32_t Block::getMagicNumber() const {
    return this->magic;
}

uint32_t Block::getSize() const {
    return this->blockSize;
}

BlockHeader Block::getHeader() const {
    return this->blockHeader;
}

uint32_t Block::getTransactionCount() const {
    return this->txnCounter;
}

uint64_t Block::getOutputsValue() const {
    uint64_t sum = 0;
    
    for (const auto & tx : this->txs) {
        sum += tx.getOutputsValue();
    }
    
    return sum;
}

std::vector<Transaction>& Block::getTransactions() {
    return txs;
}

std::vector<uint8_t> Block::calcMerkleRoot() {
    
    std::vector<uint8_t> result;    
    
    
    
    return result;
}
