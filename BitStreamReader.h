// BitStreamReader.h
#ifndef BITSTREAMREADER_H
#define BITSTREAMREADER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

class BitStreamReader {
private:
    std::ifstream& inputStream;
    unsigned int totalBitsToRead;
    unsigned int bitsReadCount = 0;

    unsigned char currentByte = 0;
    int bitOffset = 8;
    bool loadNextByte();

public:
    BitStreamReader(std::ifstream& inputStream, unsigned int totalBitsToRead);
    bool readBit();
    unsigned int getBitsReadCount() const {
        return bitsReadCount;
    }
    unsigned int getTotalBitsToRead() const {
        return totalBitsToRead;
    }
};

#endif // BITSTREAMREADER_H