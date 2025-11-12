#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H
#include <vector>
#include "huffmannode.h"
class HuffmanTree
{
public:
    HuffmanTree();
private:
    std::vector<HuffmanNode> huffmanTreeTable;
};

#endif // HUFFMANTREE_H
