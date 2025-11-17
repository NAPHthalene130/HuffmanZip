//
// Created by 13015 on 25-11-15.
//

#ifndef FILEREADERUTIL_H
#define FILEREADERUTIL_H
#include <cstring>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "huffmannode.h"
class FileReaderUtil {
public:
    const static int OPE_CREATE_DIR_AND_ENTER = 0;
    const static int OPE_CREATE_FILE_AND_WRITE = 1;
    const static int OPE_RETURN = 2;
    const static int OPE_END = 3;
    const static int TYPE_DIR = 0;
    const static int TYPE_FILE = 1;
    //读取树
    static std::vector<HuffmanNode> readTree(std::ifstream &infile) {
        if (!infile.is_open()) {
            std::cerr << "[ERROR][FileReaderUtil-readTree-1]" << "ifStream Error" << std::endl;
        }
        std::vector<HuffmanNode> tree;
        unsigned int treeSize = 0;
        infile.read(reinterpret_cast<char*>(&treeSize), sizeof(treeSize));
        if (!infile.good() || treeSize == 0) {
            std::cerr << "[ERROR][FileReaderUtil-readTree-2]" << "ifStream Error" << std::endl;
        }
        for (unsigned int i = 0; i < treeSize; i++) {
            int index = -1;
            infile.read(reinterpret_cast<char*>(&index), sizeof(index));
            int parentIndex = -1;
            infile.read(reinterpret_cast<char*>(&parentIndex), sizeof(parentIndex));
            int leftChildIndex = -1;
            infile.read(reinterpret_cast<char*>(&leftChildIndex), sizeof(leftChildIndex));
            int rightChildIndex = -1;
            infile.read(reinterpret_cast<char*>(&rightChildIndex), sizeof(rightChildIndex));
            int value = -1;
            infile.read(reinterpret_cast<char*>(&value), sizeof(value));
            int isLeaf = 0;
            infile.read(reinterpret_cast<char*>(&isLeaf), sizeof(isLeaf));
            HuffmanNode newNode;
            newNode.setIndex(index);
            newNode.setParentIndex(parentIndex);
            newNode.setLeftChildIndex(leftChildIndex);
            newNode.setRightChildIndex(rightChildIndex);
            newNode.setValue(value);
            newNode.isLeaf = (isLeaf == 1);
            tree.push_back(newNode);
        }
        return tree;
    }

    //读取频率
    static std::vector<int> readFreq(std::ifstream &infile) {
        if (!infile.is_open()) {
            std::cerr << "[ERROR][FileReaderUtil-readFreq-1]" << "ifStream Error" << std::endl;
        }
        std::vector<int> freq(256);
        unsigned int treeSize = 0;
        for (unsigned int i = 0; i < 256; i++) {
            infile.read(reinterpret_cast<char*>(&freq[i]), sizeof(freq[i]));
        }
        return freq;
    }
    //读取文件名
    static std::string readFileName(std::ifstream &infile) {
        if (!infile.is_open()) {
            std::cerr << "[ERROR][FileReaderUtil-readFileName-1]" << "ifStream Error: File not open." << std::endl;
            throw std::runtime_error("File not open.");
        }
        uint32_t length_val = 0;
        infile.read((char*)&length_val, sizeof(length_val));
        if (infile.fail()) {
            if (infile.eof()) {
                std::cerr << "[WARNING][FileReaderUtil-readFileName-2]" << "Reached EOF while trying to read file name length." << std::endl;
            } else {
                std::cerr << "[ERROR][FileReaderUtil-readFileName-3]" << "File read error while reading file name length." << std::endl;
                infile.clear();
                throw std::runtime_error("File read error while reading file name length.");
            }
            return "";
        }
        if (length_val == 0) {
            return "";
        }
        std::vector<char> buffer(length_val);
        auto expected_read = (std::streamsize)length_val;
        infile.read(buffer.data(), expected_read);
        if (infile.gcount() != expected_read) {
            if (infile.eof()) {
                std::cerr << "[WARNING][FileReaderUtil-readFileName-4]"
                          << "Reached EOF prematurely while reading string. Read " << infile.gcount()
                          << " bytes instead of " << expected_read << "." << std::endl;
            } else {
                std::cerr << "[ERROR][FileReaderUtil-readFileName-5]"
                          << "File read error or partial read while reading string." << std::endl;
                infile.clear();
                throw std::runtime_error("File read error or partial read while reading string.");
            }
        }
        std::string res(buffer.data(), infile.gcount());
        return res;
    }

    //读取操作
    static int readOpe(std::ifstream &infileStream) {
        int Ope = -1;
        if (!infileStream.is_open()) {
            std::cerr << "[ERROR][FileReaderUtil-readOpe-1]" << "ifStream Error: File not open" << std::endl;
            return -1;
        }
        infileStream.read((char*)&Ope, sizeof(int));
        if (infileStream.fail()) {
            std::cerr << "[ERROR][FileReaderUtil-readOpe-2]" << "ifStream Error: Read fail or EOF reached before reading full int" << std::endl;
        }
        return Ope;
    }

    //读取校验
    static bool readCheck(std::ifstream &infileStream) {
        if (!infileStream.is_open()) {
            std::cerr << "[ERROR][FileReaderUtil-readCheck-1]" << "ifStream Error: File not open" << std::endl;
            return false;
        }
        const char expected_magic[] = "HUFF";
        const int magic_len = 4;
        char read_magic[magic_len];
        infileStream.read(read_magic, magic_len);

        if (infileStream.fail()) {
            std::cerr << "[ERROR][FileReaderUtil-readCheck-2]" << "ifStream Error: Read fail or EOF reached before reading full magic" << std::endl;
            return false;
        }
        if (std::memcmp(read_magic, expected_magic, magic_len) != 0) {
            std::cerr << "[ERROR][FileReaderUtil-readCheck-3]" << "Magic Check Error: Expected 'HUFF', but read different bytes" << std::endl;
            return false;
        }
        return true;
    }

    //读取类型
    static int readType(std::ifstream &infileStream) {
        int type = 0;
        if (!infileStream.is_open()) {
            std::cerr << "[ERROR][FileReaderUtil-readType-1]" << "ifStream Error: File not open" << std::endl;
            return 0;
        }
        infileStream.read((char*)&type, sizeof(int));
        if (infileStream.fail()) {
            std::cerr << "[ERROR][FileReaderUtil-readType-2]" << "ifStream Error: Read fail" << std::endl;
        }
        return type;
    }
};



#endif //FILEREADERUTIL_H
