//
// Created by 13015 on 25-11-17.
//

#include "BitStreamWriter.h"
BitStreamWriter::BitStreamWriter(std::ofstream& output_stream) : ofs(output_stream) {}

void BitStreamWriter::write_bits(const std::vector<bool>& bits) {
    total_bits_written += bits.size();
    bit_buffer.insert(bit_buffer.end(), bits.begin(), bits.end());
    while (bit_buffer.size() >= 8) {
        unsigned char byte = 0;
        for (int i = 0; i < 8; ++i) {
            if (bit_buffer[i]) {
                byte |= (1 << (7 - i));
            }
        }
        // std::cout << "W" << index << ":" << (int)byte << std::endl;
        // index++;
        ofs.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
        bit_buffer.erase(bit_buffer.begin(), bit_buffer.begin() + 8);
    }
}

void BitStreamWriter::flush_and_close() {
    if (!bit_buffer.empty()) {
        padding_bits = 8 - bit_buffer.size();
        for (size_t i = 0; i < padding_bits; ++i) {
            bit_buffer.push_back(false);
        }
        unsigned char byte = 0;
        for (int i = 0; i < 8; ++i) {
            if (bit_buffer[i]) {
                byte |= (1 << (7 - i));
            }
        }
        ofs.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
    }
}