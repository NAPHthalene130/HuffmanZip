#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H
#include <vector>
#include "huffmannode.h"
class HuffmanTree
{
public:
    HuffmanTree();
    HuffmanTree(std::vector<int> freqList);
    void appendNode(HuffmanNode node);
    void createTree(std::vector<int> freqList);
private:
    int rootIndex;
    int size;
    std::vector<HuffmanNode> huffmanTree;
};

#endif // HUFFMANTREE_H
