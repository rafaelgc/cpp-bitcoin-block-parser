#include "BlockHeader.hpp"
#include "Blob.hpp"

BlockHeader::BlockHeader(std::istream &input) {
    this->init(input);
}

BlockHeader::BlockHeader() {
    this->version = timestamp = bits = nonce = 0;
}

void BlockHeader::init(std::istream &input) {
    input.read((char *)(&(this->version)), 4);
    input.read((char *)(this->prevBlock.first()), 32);    
    input.read((char *)(this->merkleRoot.first()), 32);
    input.read((char*)(&this->timestamp), 4);
    input.read((char*)(&this->bits), 4);
    input.read((char*)(&this->nonce), 4);
    
    this->calcHash();
}

int32_t BlockHeader::getVersion() const {
    return this->version;
}

uint256_t BlockHeader::getPreviousBlock() {
    return this->prevBlock;
}

uint256_t BlockHeader::getMerkleRoot() {
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

uint256_t BlockHeader::getHash() const {
    return hash;
}

std::string BlockHeader::getHashStr() const {
    return this->hash.getHexString();
}

void BlockHeader::calcHash() {
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
    
    for (unsigned int i = 0; i < hash2.size(); i++) {
        this->hash << hash2[i];
    }
}

