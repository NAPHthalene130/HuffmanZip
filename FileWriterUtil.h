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


};



#endif //FILEWIRTERUTIL_H
