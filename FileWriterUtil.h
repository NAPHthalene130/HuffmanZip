//
// Created by 13015 on 25-11-15.
//

#ifndef FILEWIRTERUTIL_H
#define FILEWIRTERUTIL_H
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <map>
#include "huffmannode.h"
#include "BitStreamWriter.h"
class FileWriterUtil {
public:
    const static int OPE_CREATE_DIR_AND_ENTER = 0;
    const static int OPE_CREATE_FILE_AND_WRITE = 1;
    const static int OPE_RETURN = 2;
    const static int OPE_END = 3;
    const static int TYPE_DIR = 0;
    const static int TYPE_FILE = 1;
    //写入树
    static void writeTree(std::ofstream &outfileStream, std::vector<HuffmanNode> tree) {
        if (!outfileStream.is_open()) {
            std::cerr << "[ERROR][FileWriterUtil-writeTree]" << "ofStream Error" << std::endl;
        }
        unsigned int treeSize = tree.size();
        outfileStream.write((char*)&treeSize, sizeof(int));
        for (int i = 0; i < tree.size(); i++) {
            int index = i;
            int parentIndex = tree[i].getParentIndex();
            int leftChildIndex = tree[i].getLeftChildIndex();
            int rightChildIndex = tree[i].getRightChildIndex();
            int value = tree[i].getValue();
            int isLeaf = tree[i].isLeaf ? 1 : 0;
            //写入格式[自身下标][双亲下标][左下标][右下标][值][是否为叶子]
            outfileStream.write((char*)&index, sizeof(int));
            outfileStream.write((char*)&parentIndex, sizeof(int));
            outfileStream.write((char*)&leftChildIndex, sizeof(int));
            outfileStream.write((char*)&rightChildIndex, sizeof(int));
            outfileStream.write((char*)&value, sizeof(int));
            outfileStream.write((char*)&isLeaf, sizeof(int));
        }
    }

    //写入文件名
    static bool writeFileName(std::ofstream &outfileStream, const std::string &data) {
        if (!outfileStream.is_open()) {
            std::cerr << "[ERROR][FileWriterUtil-writeFileName-1]" << "ofStream Error: File not open" << std::endl;
            return false;
        }
        uint32_t length_val = (uint32_t)data.length();
        outfileStream.write((const char*)&length_val, sizeof(length_val));
        if (outfileStream.fail()) {
            std::cerr << "[ERROR][FileWriterUtil-writeFileName-2]" << "File write error (length)." << std::endl;
            return false;
        }
        const char* bytes = data.data();
        auto length = (std::streamsize)length_val;
        outfileStream.write(bytes, length);
        if (outfileStream.fail()) {
            std::cerr << "[ERROR][FileWriterUtil-writeFileName-3]" << "File write error (data)." << std::endl;
            return false;
        }
        return true;
    }

    //写入操作
    static void writeOpe(std::ofstream &outfileStream, int Ope) {
        if (!outfileStream.is_open()) {
            std::cerr << "[ERROR][FileWriterUtil-writeOpe-1]" << "ofStream Error: File not open" << std::endl;
            return;
        }
        outfileStream.write((char*)&Ope, sizeof(int));
        if (outfileStream.fail()) {
            std::cerr << "[ERROR][FileWriterUtil-writeOpe-2]" << "ofStream Error: Write fail" << std::endl;
        }
    }

    //写入校验
    static void writeCheck(std::ofstream &outfileStream) {
        if (!outfileStream.is_open()) {
            std::cerr << "[ERROR][FileWriterUtil-writeCheck-1]" << "ofStream Error: File not open" << std::endl;
            return;
        }
        const char magic[] = "HUFF";
        const int magic_len = 4;
        outfileStream.write(magic, magic_len);
        if (outfileStream.fail()) {
            std::cerr << "[ERROR][FileWriterUtil-writeCheck-2]" << "ofStream Error: Write fail" << std::endl;
        }
    }

    //写入类型
    static void writeType(std::ofstream &outfileStream, int type) {
        if (!outfileStream.is_open()) {
            std::cerr << "[ERROR][FileWriterUtil-writeType-1]" << "ofStream Error: File not open" << std::endl;
            return;
        }
        outfileStream.write((char*)&type, sizeof(int));
        if (outfileStream.fail()) {
            std::cerr << "[ERROR][FileWriterUtil-writeType-2]" << "ofStream Error: Write fail" << std::endl;
        }
    }

    //写入文件
static void writeFile(std::ofstream &outfileStream, const std::map<unsigned char,
        std::vector<bool>>& bitMap, const std::string& inFilePath) {
        const size_t BITS_FIELD_SIZE = sizeof(unsigned int);
        unsigned int placeholder = 0;
        outfileStream.write(reinterpret_cast<const char*>(&placeholder), BITS_FIELD_SIZE);
        if (outfileStream.fail()) {
            std::cerr << "[ERROR][FileWriterUtil-writeFile-Header]" << "File write error (placeholder)." << std::endl;
            return;
        }
        std::ifstream ifs(inFilePath, std::ios::binary);
        if (!ifs.is_open()) {
            std::cerr << "[ERROR][FileWriterUtil-writeFile-1]" << "ifStream Error: Read file error" << std::endl;
            return;
        }
        BitStreamWriter bitOutfileWriter(outfileStream);
        constexpr size_t BUFFER_SIZE = 1024;
        std::vector<unsigned char> buffer(BUFFER_SIZE);
        while (ifs.good()) {
            ifs.read(reinterpret_cast<char*>(buffer.data()), BUFFER_SIZE);
            size_t bytesRead = ifs.gcount();
            if (bytesRead == 0) {
                break;
            }
            for (size_t i = 0; i < bytesRead; ++i) {
                unsigned char byte_in = buffer[i];
                auto it = bitMap.find(byte_in);
                if (it != bitMap.end()) {
                    bitOutfileWriter.write_bits(it->second);
                } else {
                    std::cerr << "[ERROR][FileWriterUtil-writeFile-2]" <<
                        "BitOfStream Error: Bytes not in bitMap" << std::endl;
                    ifs.close();
                    bitOutfileWriter.flush_and_close();
                    return;
                }
            }
        }
        bitOutfileWriter.flush_and_close();
        ifs.close();
        unsigned int total_bits = bitOutfileWriter.get_total_bits_written();
        std::streampos current_pos = outfileStream.tellp();
        outfileStream.seekp(0, std::ios::beg);
        outfileStream.write(reinterpret_cast<const char*>(&total_bits), BITS_FIELD_SIZE);
        if (outfileStream.fail()) {
            std::cerr << "[ERROR][FileWriterUtil-writeFile-TotalBits]" << "File write error (total bits)." << std::endl;
        }
        outfileStream.seekp(current_pos);
    }
};



#endif //FILEWIRTERUTIL_H
