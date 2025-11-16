//
// Created by 13015 on 25-11-15.
//

#ifndef FILEWIRTERUTIL_H
#define FILEWIRTERUTIL_H
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include "huffmannode.h"
class FileWriterUtil {
public:
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
};



#endif //FILEWIRTERUTIL_H
