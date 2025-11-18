#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>

#ifndef BITSTREAMWRITER_H
#define BITSTREAMWRITER_H


#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
class BitStreamWriter {
private:
    std::ofstream& ofs;
    std::vector<bool> bit_buffer;
    unsigned char padding_bits = 0; // 记录最后一个字节的填充位数
    unsigned int total_bits_written = 0;
    int index = 0;
public:
    BitStreamWriter(std::ofstream& output_stream);
    void write_bits(const std::vector<bool>& bits);
    void flush_and_close();
    unsigned char get_padding_bits() const {
        return padding_bits;
    }
    unsigned int get_total_bits_written() const {
        return total_bits_written;
    }
};

#endif //BITSTREAMWRITER_H
