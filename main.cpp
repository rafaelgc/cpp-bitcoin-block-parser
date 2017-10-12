#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include "picosha2.h"

/* HELPERS */

#define TOP_4 0xF0
#define BOT_4 0x0F

uint64_t varint(std::istream &input) {
    uint8_t lengthId = 0;
    uint64_t res = 0;
    
    input.read((char *)(&lengthId), 1);
    if (lengthId < 0xFD) {
        return lengthId;
    }
    else if (lengthId == 0xFD) {
        input.read((char *)(&res), 2);
    }
    else if (lengthId == 0xFE) {
        input.read((char *)(&res), 4);
    }
    else if (lengthId == 0xFF) {
        input.read((char *)(&res), 8);
    }
    else {
        std::cout << "Unsupported varint" << std::endl;
    }
    
    return res;
}

char hexTable[] = {'0', '1', '2', '3', '4', '5',
                '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char uint8ToHexBot(uint8_t hex) {
    return hexTable[hex & BOT_4];
}

char uint8ToHexTop(uint8_t hex) {
    return hexTable[(hex & TOP_4) >> 4];
}

/* INPUT */

class Input {
    private:
    uint8_t prevTransaction[32];
    uint32_t txoutIndex;
    uint64_t scriptLength;
    std::vector<uint8_t> scriptSig;
    uint32_t sequenceNumber;
    
    public:
    Input(std::istream & input);
    void init(std::istream & input);
    
    uint32_t getSequenceNumber() const;
};

Input::Input(std::istream & input) {
    this->init(input);
}

void Input::init(std::istream & input) {
    input.read((char *)(&this->prevTransaction), 32);    
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

/* OUTPUT */

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

/* TRANSACTION */

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

/* BLOCK HEADER */

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

BlockHeader::BlockHeader(std::istream &input) {
    this->init(input);
}

BlockHeader::BlockHeader() {
    this->version = timestamp = bits = nonce = 0;
}

void BlockHeader::init(std::istream &input) {
    input.read((char *)(&(this->version)), 4);
    input.read((char *)(&this->prevBlock), 32);    
    input.read((char *)(&this->merkleRoot), 32);
    input.read((char*)(&this->timestamp), 4);
    input.read((char*)(&this->bits), 4);
    input.read((char*)(&this->nonce), 4);
}

int32_t BlockHeader::getVersion() const {
    return this->version;
}

int8_t* BlockHeader::getPreviousBlock() {
    return this->prevBlock;
}

int8_t* BlockHeader::getMerkleRoot() {
    return this->merkleRoot;
}

int32_t BlockHeader::getTimestamp() const {
    return this->timestamp;
}

int32_t BlockHeader::getBits() const {
    return this->bits;
}

int32_t BlockHeader::getNonce() const {
    return this->nonce;
}

std::string BlockHeader::getHash() {
    if (this->hashStr.empty()) {
        std::vector<uint8_t> block;
        uint8_t i = 0;
        
        uint8_t *pointer = (uint8_t *)&version;
        for (i = 0; i < 4; i++) {
            block.push_back(*pointer);
            pointer++;
        }
        
        pointer = (uint8_t *)&prevBlock;
        for (i = 0; i < 32; i++) {
            block.push_back(*pointer);
            pointer++;
        }
        
        pointer = (uint8_t *)&merkleRoot;
        for (i = 0; i < 32; i++) {
            block.push_back(*pointer);
            pointer++;
        }
        
        pointer = (uint8_t *)&timestamp;
        for (i = 0; i < 4; i++) {
            block.push_back(*pointer);
            pointer++;
        }
        
        pointer = (uint8_t *)&bits;
        for (i = 0; i < 4; i++) {
            block.push_back(*pointer);
            pointer++;
        }
        
        pointer = (uint8_t *)&nonce;
        for (i = 0; i < 4; i++) {
            block.push_back(*pointer);
            pointer++;
        }
        
        picosha2::hash256_one_by_one hasher;
        
        //First SHA-256
        hasher.process(block.begin(), block.end());
        hasher.finish();
        
        std::vector<uint8_t> hash(32);
        hasher.get_hash_bytes(hash.begin(), hash.end());
        
        //Second SHA-256
        picosha2::hash256_one_by_one hasher2;
        
        hasher2.process(hash.begin(), hash.end());
        hasher2.finish();
        
        std::vector<uint8_t> hash2(32);
        hasher2.get_hash_bytes(hash2.begin(), hash2.end());
        
        this->hashStr = picosha2::get_hash_hex_string(hasher2);
    }
    
    return this->hashStr;
}

/* BLOCK */

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
};

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

uint32_t Block::getTransactionCounter() const {
    return this->txnCounter;
}

uint64_t Block::getOutputsValue() const {
    uint64_t sum = 0;
    
    for (const auto & tx : this->txs) {
        sum += tx.getOutputsValue();
    }
    
    return sum;
}



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
            std::cout << input.tellg() << std::endl << std::endl;
            counter++;
        }
    }
    
    std::cout << "Blocks: " << std::dec << counter << std::endl;
    
    input.close();
    
    return 0;
}

