#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H
#include <vector>
#include <map>
#include "huffmannode.h"
class HuffmanTree
{
public:
    HuffmanTree();
    HuffmanTree(std::vector<int> freqList);
    HuffmanTree(const std::vector<HuffmanNode>& treeVector);
    void appendNode(HuffmanNode node);
    void createTree(std::vector<int> freqList);
    std::vector<HuffmanNode> getTree();
    std::map<unsigned char, std::vector<bool>> getBitMap();
    int getrootIndex();
private:
    int rootIndex;
    int size;
    std::vector<HuffmanNode> huffmanTree;
    std::map<unsigned char, std::vector<bool>> charToBit;
};

#endif // HUFFMANTREE_H
