//
// Created by 13015 on 25-11-15.
//

#ifndef FILEREADERUTIL_H
#define FILEREADERUTIL_H
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
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
};



#endif //FILEREADERUTIL_H
