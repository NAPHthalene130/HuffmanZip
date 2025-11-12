#include "ziputil.h"
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
    std::vector<int> read;
    int BUFFER_SIZE = 1024;
    std::fstream inputFile(filePath, std::ios::in | std::ios::binary);

    if (!inputFile.is_open()) {
        std::cerr << "[ERROR][ZipUtil-calFileFreq-1]: Can't open file: " << filePath << std::endl;
    }

    fs::path p(filePath);
    if (fs::is_regular_file(p)) {
        //当前路径是文件
        std::vector<char> buffer(BUFFER_SIZE);
        while (inputFile.read(buffer.data(), BUFFER_SIZE)) {
            for (unsigned char c : buffer) {
                freq[c]++;
            }
            read.insert(read.end(), buffer.begin(), buffer.end());
        }
        if (inputFile.gcount() > 0) {
            size_t bytesRead = inputFile.gcount();
            freq.insert(freq.end(), buffer.begin(), buffer.begin() + bytesRead);
            for (size_t i = 0; i < bytesRead; ++i) {
                read.push_back(buffer[i]);
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
    std::vector<int> freq(256,0);
    calFileFreq(freq,filePath);
    for (int i = 0; i < 256; i++) {
        std::cout << i << ": " << freq[i] << std::endl;
    }
}
