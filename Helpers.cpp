#include "Helpers.hpp"
char hexTable[] = {'0', '1', '2', '3', '4', '5',
                '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
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

char uint8ToHexBot(uint8_t hex) {
    return hexTable[hex & BOT_4];
}

char uint8ToHexTop(uint8_t hex) {
    return hexTable[(hex & TOP_4) >> 4];
}
