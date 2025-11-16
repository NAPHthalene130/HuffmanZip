//
// Created by 13015 on 25-11-15.
//

#ifndef FILEREADERUTIL_H
#define FILEREADERUTIL_H
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "huffmannode.h"
class FileReaderUtil {
public:
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
};



#endif //FILEREADERUTIL_H
