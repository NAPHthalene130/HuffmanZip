#include "ziputil.h"
#include "huffmantree.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
ZipUtil::ZipUtil() {}
ZipUtil::~ZipUtil() {}

void ZipUtil::calFileFreq(std::vector<int> &freq, std::string filePath)
{
    namespace fs = std::filesystem;
    int BUFFER_SIZE = 1024;
    std::fstream inputFile(filePath, std::ios::in | std::ios::binary);
    std::cout << "TEST1:" << filePath << std::endl;
    fs::path p(filePath);
    if (!fs::exists(p)) {
        std::cerr << "[ERROR][ZipUtil-calFileFreq-1]: Path is not existing" << filePath << std::endl;
    }
    if (fs::is_regular_file(p)) {
        //当前路径是文件
        std::vector<char> buffer(BUFFER_SIZE);
        while (inputFile.read(buffer.data(), BUFFER_SIZE)) {
            for (unsigned char c : buffer) {
                freq[c]++;
            }
        }
        if (inputFile.gcount() > 0) {
            size_t bytesRead = inputFile.gcount();
            for (size_t i = 0; i < bytesRead; ++i) {
                freq[(unsigned char)buffer[i]]++;
            }
        }
        inputFile.close();
    } else if (fs::is_directory(p)) {
        //当前路径是文件夹
        try {
            for (const auto& entry : fs::directory_iterator(p)) {
                calFileFreq(freq, entry.path().string());
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "[ERROR][ZipUtil-calFileFreq-2]: Traveling files error" << e.what() << std::endl;
        }
    } else {
        //其他情况
    }

}

ZipUtil &ZipUtil::getInstance()
{
    static ZipUtil instance;
    return instance;
}

//解压文件
void ZipUtil::deCode(const std::string filePath)
{

}

//压缩文件
void ZipUtil::enCode(const std::string filePath)
{
    using std::cout;
    using std::endl;
    std::vector<int> freq(256,0);
    calFileFreq(freq,filePath);
    HuffmanTree huffTree(freq);
    std::map<unsigned char, std::vector<bool>> bitMap = huffTree.getBitMap();
    for (int i = 0 ; i < huffTree.getTree().size(); i++) {
        HuffmanNode node = huffTree.getTree()[i];
        cout <<"Index:"<<i<<" Freq:"<<node.getFreq()<<" Parent:"<<node.getParentIndex()<<" LChild" << node.getLeftChildIndex() << " RChild:"<<node.getRightChildIndex()<<endl;
    }

    for (int i = 0 ; i < 256; i++) {
        if (bitMap.find((unsigned char)i) == bitMap.end()) {continue;}
        else cout << i << ": ";
        for (bool value: bitMap[(unsigned char)i]) {
            if (value) {
                cout << 1;
            } else {
                cout << 0;
            }
        }
        cout << endl;
    }

    //测试
    std::string testLine = "abcdefg";
    for (int i = 0 ; i < testLine.size(); i++) {
        char nowChar = testLine[i];
        std::vector<bool> boolPath = bitMap[nowChar];
        int nowIndex = huffTree.getrootIndex();
        for (bool nowBool : boolPath) {
            if (nowBool) {
                nowIndex = huffTree.getTree()[nowIndex].getRightChildIndex();
            } else {
                nowIndex = huffTree.getTree()[nowIndex].getLeftChildIndex();
            }
        }
        cout << huffTree.getTree()[nowIndex].getValue();
    }
    cout << endl;
}
