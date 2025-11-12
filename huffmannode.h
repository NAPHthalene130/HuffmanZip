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
    void setIndex(int index);
    int getLeftChildIndex();
    int getRightChildIndex();
    int getParentIndex();
    int getFreq();
    int getIndex();
    struct freqCmp {
        bool operator() (const HuffmanNode node1, const HuffmanNode node2) const {
            return node1.freq > node2.freq;
        }
    };

private:
    int parentIndex = -1;
    int leftChildIndex = -1;
    int rightChildIndex = -1;
    int freq = 0;
    char value;
    bool isLeaf;
    int myIndex = -1;
};

#endif // HUFFMANNODE_H
