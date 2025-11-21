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
HuffmanTree::HuffmanTree(const std::vector<HuffmanNode>& treeVector)
    : huffmanTree(treeVector)
{
    this->size = huffmanTree.size();

    this->rootIndex = -1;
    for (auto& node : this->huffmanTree) {
        if (node.getParentIndex() == -1) {
            this->rootIndex = node.getIndex();
            break;
        }
    }

    if (this->huffmanTree.empty()) {
        return;
    }

    for (int i = 0 ; i < huffmanTree.size(); i++) {
        if (!huffmanTree[i].isLeaf) {
            continue;
        }

        // This is a leaf node, build its code
        int nowIndex = i;
        while (nowIndex != -1) {
            int pIndex = huffmanTree[nowIndex].getParentIndex();
            if (pIndex == -1) break;

            if (nowIndex == huffmanTree[pIndex].getLeftChildIndex()) {
                //current node is parent's left child
                charToBit[huffmanTree[i].getValue()].push_back(0);
            } else if (nowIndex == huffmanTree[pIndex].getRightChildIndex()) {
                //current node is parent's right child
                charToBit[huffmanTree[i].getValue()].push_back(1);
            } else {
                std::cerr << "[ERROR][HuffmanTree-Constructor(Vector)]-1]: Unknow Index" << std::endl;
            }
            nowIndex = pIndex;
        }
        // Reverse the code since it was built from leaf to root
        std::reverse(charToBit[huffmanTree[i].getValue()].begin(),charToBit[huffmanTree[i].getValue()].end());
    }
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
            unsigned char c =  (unsigned char)i;
            HuffmanNode newNode(freqList[i], c);
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

    for (int i = 0 ; i < 256 && i < huffmanTree.size(); i++) {
        if (!huffmanTree[i].isLeaf) {
            continue;
        }
        int nowIndex = i;
        while (nowIndex != -1) {
            int pIndex = huffmanTree[nowIndex].getParentIndex();
            if (pIndex == -1) break;
            if (nowIndex == huffmanTree[pIndex].getLeftChildIndex()) {
                //当前节点是父节点的左节点
                charToBit[huffmanTree[i].getValue()].push_back(0);
            } else if (nowIndex == huffmanTree[pIndex].getRightChildIndex()) {
                //当前节点是父节点的右节点
                charToBit[huffmanTree[i].getValue()].push_back(1);
            } else {
                std::cerr << "[ERROR][HuffmanTree-createTree-1]: Unknow Index" << std::endl;
            }
            nowIndex = pIndex;
        }
        std::reverse(charToBit[huffmanTree[i].getValue()].begin(),charToBit[huffmanTree[i].getValue()].end());
    }

}

std::vector<HuffmanNode> HuffmanTree::getTree()
{
    return this->huffmanTree;
}

std::map<unsigned char, std::vector<bool> > HuffmanTree::getBitMap()
{
    return this->charToBit;
}

int HuffmanTree::getrootIndex()
{
    return this->rootIndex;
}
