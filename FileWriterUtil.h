//
// Created by 13015 on 25-11-15.
//

#ifndef FILEWIRTERUTIL_H
#define FILEWIRTERUTIL_H
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include "huffmannode.h"
class FileWriterUtil {
    static void writeTree(std::ofstream &outfile, std::vector<HuffmanNode> tree) {
        if (!outfile.is_open()) {
            std::cerr << "[ERROR][FileWriterUtil-writeTree]" << "ofStream Error" << std::endl;
        }
        for (int i = 0; i < tree.size(); i++) {
            int index = i;
            int parentIndex = tree[i].getParentIndex();
            int leftChildIndex = tree[i].getLeftChildIndex();
            int rightChildIndex = tree[i].getRightChildIndex();
            int value = tree[i].getValue();
            int isLeaf = tree[i].isLeaf ? 1 : 0;
            //写入格式[自身下标][双亲下标][左下标][右下标][值][是否为叶子]
            outfile.write((char*)&index, sizeof(int));
            outfile.write((char*)&parentIndex, sizeof(int));
            outfile.write((char*)&leftChildIndex, sizeof(int));
            outfile.write((char*)&rightChildIndex, sizeof(int));
            outfile.write((char*)&value, sizeof(int));
            outfile.write((char*)&isLeaf, sizeof(int));
        }
    }


};



#endif //FILEWIRTERUTIL_H
