#include "huffmantree.h"
#include "huffmannode.h"
#include <queue>
#include <iostream>
#include <algorithm>
HuffmanTree::HuffmanTree() {}

HuffmanTree::HuffmanTree(std::vector<int> freqList)
{
    createTree(freqList);
}

void HuffmanTree::appendNode(HuffmanNode node)
{
    this->huffmanTree.push_back(node);
}

void HuffmanTree::createTree(std::vector<int> freqList)
{
    using std::priority_queue;
    using std::deque;
    priority_queue<HuffmanNode, deque<HuffmanNode>, HuffmanNode::freqCmp> pq;
    bool flag = false;
    for (int value : freqList) {
        if (value != 0) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        return;
    }
    int index = 0;
    for (int i = 0 ; i < freqList.size(); i++) {
        if (freqList[i] != 0) {
            HuffmanNode newNode(freqList[i], (unsigned char)i);
            newNode.setIndex(index);
            pq.push(newNode);
            this->appendNode(newNode);
            index++;
        }
    }
    while (pq.size() > 1) {
        HuffmanNode node1 = pq.top(); pq.pop();
        HuffmanNode node2 = pq.top(); pq.pop();
        HuffmanNode pNode = (node1.getFreq()+ node2.getFreq());
        pNode.setLeftChildIndex(node1.getIndex());
        pNode.setRightChildIndex(node2.getIndex());
        pNode.setIndex(index);
        this->huffmanTree[node1.getIndex()].setParentIndex(pNode.getIndex());
        this->huffmanTree[node2.getIndex()].setParentIndex(pNode.getIndex());
        pq.push(pNode);
        this->appendNode(pNode);
        index++;
    }
    HuffmanNode rootNode = pq.top(); pq.pop();
    this->rootIndex = rootNode.getIndex();

    for (int i = 0 ; i < 255; i++) {
        int nowIndex = i;
        while (nowIndex != -1) {
            int pIndex = huffmanTree[nowIndex].getParentIndex();
            if (pIndex == -1) break;
            if (nowIndex == huffmanTree[pIndex].getLeftChildIndex()) {
                //当前节点是父节点的左节点
                charToBit[(unsigned char)i].push_back(0);
            } else if (nowIndex == huffmanTree[pIndex].getRightChildIndex()) {
                //当前节点是父节点的右节点
                charToBit[(unsigned char)i].push_back(1);
            } else {
                std::cerr << "[ERROR][HuffmanTree-createTree-1]: Unknow Index" << std::endl;
            }
            nowIndex = pIndex;
        }
        std::reverse(charToBit[(unsigned char)i].begin(),charToBit[(unsigned char)i].end());
    }

}

std::vector<HuffmanNode> HuffmanTree::getTree()
{
    return this->huffmanTree;
}
