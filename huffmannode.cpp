#include "huffmannode.h"

HuffmanNode::HuffmanNode() {}

void HuffmanNode::setIndex(int index)
{
    this->myIndex = index;
}

int HuffmanNode::getFreq()
{
    return this->freq;
}

int HuffmanNode::getIndex()
{
    return this->myIndex;
}
