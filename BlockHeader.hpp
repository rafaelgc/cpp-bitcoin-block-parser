#ifndef BLOCKHEADER_HPP
#define BLOCKHEADER_HPP

#include <stdint.h>
#include <fstream>
#include "picosha2.h"

class BlockHeader {
    private:
    int32_t version;
    int8_t prevBlock[32];
    int8_t merkleRoot[32];
    int32_t timestamp, bits, nonce/*, txnCount*/;
    std::string hashStr;
    public:
    BlockHeader(std::istream &input);
    BlockHeader();
    
    void init(std::istream &input);
    
    int32_t getVersion() const;
    int8_t* getPreviousBlock();
    int8_t* getMerkleRoot();
    int32_t getTimestamp() const;
    int32_t getBits() const;
    int32_t getNonce() const;
    
    std::string getHash();
};

#endif
