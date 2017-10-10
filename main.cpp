#include <iostream>
#include <fstream>
#include <stdint.h>

#define TOP_4 0xF0
#define BOT_4 0x0F

char hexTable[] = {'0', '1', '2', '3', '4', '5',
                '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char uint8ToHexBot(uint8_t hex) {
    return hexTable[hex & BOT_4];
}

char uint8ToHexTop(uint8_t hex) {
    return hexTable[(hex & TOP_4) >> 4];
}

uint32_t varint(std::ifstream &input) {
    uint8_t lengthId = 0;
    
    input.read((char *)(&lengthId), 1);
    if (lengthId & 0xFD == 0xFD) {
        uint32_t output = 0;
        input.read((char*)(&output), 1);
        return output;
    }
    else {
        std::cout << "Unsupported varint";
        return 0;
    }
}

int main(int argc, char ** argv) {
    if (argc < 2) {
        std::cout << "Command: " << argv[0] << " FILENAME" << std::endl;
    }
    

    std::ifstream input(argv[1], std::ios::binary);
    
    uint32_t magic = 0, size = 0, version = 0, timestamp = 0, diffTarget = 0, nonce = 0;
    uint32_t transactions = 0;
    uint8_t prevHash[32], merkleRoot[32];
    uint8_t lengthId = 0;
    
    input.read((char *)(&magic), 4);
    input.read((char *)(&size), 4);
    
    /* HEADER */
    
    input.read((char *)(&version), 4);
    input.read((char *)(&prevHash), 32);    
    input.read((char *)(&merkleRoot), 32);
    input.read((char*)(&timestamp), 4);
    input.read((char*)(&diffTarget), 4);
    input.read((char*)(&nonce), 4);
    
    /* END HEADER*/
    
    transactions = varint(input);
    
    /*TRANSACTIONS*/
    
    uint32_t transVersion = 0;
    input.read((char *)(&transVersion), 4);
    
    uint32_t inputs = varint(input);
    
    /*INPUTS*/
    
    uint32_t outputs = varint(input);
    
    uint32_t locktime = 0;
    input.read((char*)(&locktime), 4);
    
    std::cout << "Magic:\t" << std::hex << magic << std::endl;
    std::cout << "Size:\t" << std::dec << size << std::endl;
    std::cout << "Version:\t" << version << std::endl;
    std::cout << "Timestamp:\t" << timestamp << std::endl;
    std::cout << "Diff:\t" << diffTarget << std::endl;
    std::cout << "Nonce: " << nonce << std::endl;
    
     std::cout << "Previous block hash: " << std::endl;
     for (int i = 0; i <32; i++) {
        std::cout << uint8ToHexTop(prevHash[31 - i]) << uint8ToHexBot(prevHash[31 - i]);
    }
    
    std::cout << std::endl;
    std::cout << "Merkle root: " << std::endl;
    
    for (int i = 0; i <32; i++) {
        std::cout << uint8ToHexTop(merkleRoot[31 - i]) << uint8ToHexBot(merkleRoot[31 - i]);
    }
    std::cout << std::endl;
    
    
    
    std::cout << std::dec << "Transactions: " << (int)transactions << std::endl;
    
    std::cout << "TRANSACTIONS" << std::endl;
    
    std::cout << "Version: " << (int)transVersion << std::endl;
    std::cout << "Inputs: " << inputs << std::endl;
    
    std::cout << "Outputs: " << outputs << std::endl;
    std::cout << "Locktime: " << locktime << std::endl;
    
    input.read((char*)(&magic), 4);
    std::cout << "Magic: " << magic << std::endl;
    
    input.read((char*)(&magic), 4);
    std::cout << "Magic: " << magic << std::endl;
    
    input.read((char*)(&magic), 4);
    std::cout << "Magic: " << magic << std::endl;
    
    input.read((char*)(&magic), 4);
    std::cout << "Magic: " << magic << std::endl;
    
    input.read((char*)(&magic), 4);
    std::cout << "Magic: " << magic << std::endl;
    
    input.read((char*)(&magic), 4);
    std::cout << "Magic: " << magic << std::endl;
    
    input.read((char*)(&magic), 4);
    std::cout << "Magic: " << magic << std::endl;
    
    // 1111 1101
    // 1111 1101 &
    // 
    
    input.close();
    
    return 0;
    /*
    
    char blockSize[4], version[4];
    uint32_t bS = 0, v = 0;
    
    input.read(blockSize, 4);
    
    
    put(&bS, blockSize, 4);
    put(&v, version, 4);
    std::cout << bS << std::endl;
    std::cout << v << std::endl;
    */
    
    /*std::cout << "Block size:\t" << blockSize << std::endl;
    std::cout << "Version:\t\t" << version << std::endl;*/
    
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
