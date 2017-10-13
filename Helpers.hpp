#ifndef HELPERS_HPP
#define HELPERS_HPP

#define TOP_4 0xF0
#define BOT_4 0x0F

#include <stdint.h>
#include <fstream>
#include <iostream>

extern char hexTable[];

uint64_t varint(std::istream &input);

char uint8ToHexBot(uint8_t hex);

char uint8ToHexTop(uint8_t hex);

#endif
