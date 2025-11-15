#include "huffmannode.h"

void HuffmanNode::setIndex(int index)
{
    this->myIndex = index;
}

void HuffmanNode::setValue(unsigned char value) {
    this->value = value;
}

int HuffmanNode::getFreq()
{
    return this->freq;
}

int HuffmanNode::getIndex()
{
    return this->myIndex;
}

unsigned char HuffmanNode::getValue()
{
    return this->value;
}

//默认构造
HuffmanNode::HuffmanNode() {

}

//叶子构造
HuffmanNode::HuffmanNode(int freq, unsigned char value)
{
    this->freq = freq;
    this->value = value;
    isLeaf = true;
}

//中间节点构造
HuffmanNode::HuffmanNode(int freq)
{
    this->freq = freq;
    isLeaf = false;
}

//set区
void HuffmanNode::setLeftChildIndex(int index)
{
    this->leftChildIndex = index;
}

void HuffmanNode::setRightChildIndex(int index)
{
    this->rightChildIndex = index;
}

void HuffmanNode::setParentIndex(int index)
{
    this->parentIndex = index;
}

//get区
int HuffmanNode::getLeftChildIndex()
{
    return this->leftChildIndex;
}

int HuffmanNode::getRightChildIndex()
{
    return this->rightChildIndex;
}

int HuffmanNode::getParentIndex()
{
    return this->parentIndex;
}
