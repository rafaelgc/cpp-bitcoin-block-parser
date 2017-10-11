#include <iostream>
#include <fstream>
#include <stdint.h>
#include <vector>

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
    
    std::cout << std::dec << value << std::endl;
        
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
    uint32_t getBlockSize() const;
    BlockHeader getBlockHeader() const;
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

uint32_t Block::getBlockSize() const {
    return this->blockSize;
}

BlockHeader Block::getBlockHeader() const {
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
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        std::cout << "Command: " << argv[0] << " FILENAME" << std::endl;
    }
    

    std::ifstream input(argv[1], std::ios::binary);
    
    uint32_t counter = 0;
    
    while (!input.eof()) {
        Block block(input);
        
        std::cout << std::hex << block.getMagicNumber() << std::endl;
        std::cout << "Output Shatoshi: " << std::dec << block.getOutputsValue() << std::endl;
        counter++;
        
        break;
    }
    
    std::cout << "Blocks: " << std::dec << counter << std::endl;
    
    input.close();
    
    return 0;
}

/*
02000020    SIZE
14c2a9b7    VERS
5c44c656d5720f694c32d97aa3354d2d926a80010000000000000000c28a2cdd PREV. BLOCK
8aeb39b66bcf7906b26c9b1b865dc50ed99243b152f4a498e883f6870fa05558 MERK. ROOT
858b0318    TIME
c67e592efdbc0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4503f0c506fabe6d6d42d107e4e8f8553a8c3b028d6905befaf12aabbce13aa63fa70d339589e6b22b0100000000000000026507013cce42401365007a27422f736c7573682f000000000166757d4b000000001976a9147c154ed1dc59609e3d26abb2df2ea3d587cd8c4188ac000000000100000001c20a2889dda7881251e3aa03f243d900dc76a17f51293685e249ffb0bf0df121000000006a47304402204f7fb0b1e0d154db27dbdeeeb8db7b7d3b887a33e712870503438d8be2d66a0102204782a2714215dc0d581e1d435b41bc6eced2c213c9ba0f993e7fcf468bb5d3110121025840d511c4bc6690916270a54a6e9290fab687f512c18eb2df0428fa69a26299ffffffff013b900200000000001976a9147c4338dea7964947b3f0954f61ef40502fe8f79188ac00000000010000000156636c7feef9e45cf82b21aaa61046d3baee7dfc9e024bb3d035e0bf5b995e82000000006a47304402204d3fb9518d45fcb8e25e0b39e585d594a78a950a46872b47f2a020b8a71f53c10220031a1fe6e431003bc57985c24de1a65a8e206a050bb9b989b8997746a07d1396012103727beb261a0796e30f0965354c7bab133d9bb6e7294553e98e9e0d4571ab831fffffffff02002d3101000000001976a914e279d48929e7f830d19bd972c035ff4b9c7c0aa188ace877a621000000001976a914244d3bb007d68c5850a0863b8c2e9e05a3f27e6588ac00000000010000000103e0d2eb298677c280ddd638aa95c80a1c6466070aebfaf45509acf9dbdcc2d7010000006a473044022016443991742d84994ee461cca11ffd4a341d29e16d47288626295d166491477602204047b601a7d0000a0026a119036958614d8936937caa1fbb068eab6bea7d3ef30121033108de5530639175e20ea6922a74dac479caced0e261af3b7b0113249c4c307fffffffff022f910b00000000001976a9148d075d55d2d8f8d1e0a2411178cde2e76c81278e88aca6433b00000000001976a9147b89794f1a8fad4396f5ff05bea666c9ae63c0e488ac00000000010000000108c47a674e1dae043b4ef3e6fd08af3f733ec137ffbce841f65df838bd7bc46a000000006a473044022010347af8f4ebab2bad61121ae2a153014227a50e7878778ac1e0a81e3057ab0402204a27a5ac50dd35c9f3f2edb31d169b9cbd38fc3d7a05cad9a6eecb47e00332ee012103ae5c519f26ba37bef7d54353d0ab60008e9d86301e31deed838ac44e34c0f2e7ffffffff0244c7782e000000001976a914561af75ee8990152da1b2aad036cda7331a9240288ac22e1c704000000001976a914e36f489e1ab8


*/
