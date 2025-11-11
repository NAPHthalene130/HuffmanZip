#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

class HuffmanNode
{
public:
    HuffmanNode();
    HuffmanNode(int freq, char value);
    HuffmanNode(int freq);
    void setLeftChildIndex(int index);
    void setRightChildIndex(int index);
    void setParentIndex(int index);
    int getLeftChildIndex();
    int getRightChildIndex();
    int getParentChildIndex();
private:
    int parentIndex;
    int leftChildIndex;
    int rightChildIndex;
    int freq;
    char value;
    bool isLeaf;
};

#endif // HUFFMANNODE_H
