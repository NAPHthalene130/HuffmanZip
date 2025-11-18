// BitStreamReader.cpp

#include "BitStreamReader.h"
#include <algorithm>

BitStreamReader::BitStreamReader(std::ifstream& inputStream, unsigned int totalBitsToRead)
    : inputStream(inputStream), totalBitsToRead(totalBitsToRead) {
    inputStream.clear();
}
bool BitStreamReader::loadNextByte() {
    if (inputStream.read(reinterpret_cast<char*>(&currentByte), sizeof(currentByte))) {
        bitOffset = 0;
        return true;
    }
    return false;
}
bool BitStreamReader::readBit() {
    if (bitsReadCount >= totalBitsToRead) {
        return false;
    }
    if (bitOffset == 8) {
        if (!loadNextByte()) {
            return false;
        }
    }
    bool bit = (currentByte & (1 << (7 - bitOffset))) != 0;
    bitOffset++;
    bitsReadCount++;

    return bit;
}