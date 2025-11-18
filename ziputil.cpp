#include "ziputil.h"
#include "huffmantree.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include "FileReaderUtil.h"
#include "FileWriterUtil.h"
ZipUtil::ZipUtil() {}
ZipUtil::~ZipUtil() {}

void ZipUtil::calFileFreq(std::vector<int> &freq, std::string filePath)
{
    namespace fs = std::filesystem;
    int BUFFER_SIZE = 1024;
    std::fstream inputFile(filePath, std::ios::in | std::ios::binary);
    // std::cout << "TEST1:" << filePath << std::endl;
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

/*
* 文件写入格式:
*  [校验][哈夫曼树数据][指令][指令]...
*  指令: 0-创建并进入目录  1-创建并写入文件  2-返回上级目录  3-结束
*  指令0: [文件名长度][文件名内容]
*  指令1: [文件名长度][文件名内容][编码实际长度][编码内容]
*/
//解压文件
void ZipUtil::deCode(const std::string& filePath, const std::string& outputPath)
{

}

void ZipUtil::deCodeTest(const std::string& filePath, const std::string& outputPath) {
    namespace fs = std::filesystem;
    using std::cout;
    using std::endl;
    using std::cerr;
    std::ifstream infile(filePath,std::ios::in | std::ios::binary);
    bool check = FileReaderUtil::readCheck(infile);
    if (!check) {
        cerr << "[WARN] CHECK FAIL" << endl;
    }
    std::vector<int> freq = FileReaderUtil::readFreq(infile);
    HuffmanTree huffTree(freq);
    std::map<unsigned char, std::vector<bool>> bitMap = huffTree.getBitMap();
    int type = FileReaderUtil::readType(infile);
    if (type == 0) {
        cout << "TYPE: DIR" << endl;
    } else {
        cout << "TYPE: FILE" << endl;
    }
    int ope = FileReaderUtil::readOpe(infile);
    cout << "ope: " << ope << endl;
    std::string name = FileReaderUtil::readFileName(infile);
    cout << "name: " << name << endl;
}

//压缩文件
void ZipUtil::enCode(const std::string& filePath, const std::string& outputPath)
{
    namespace fs = std::filesystem;
    using std::cout;
    using std::endl;
    std::vector<int> freq(256,0);
    calFileFreq(freq,filePath);
    HuffmanTree huffTree(freq);
    std::map<unsigned char, std::vector<bool>> bitMap = huffTree.getBitMap();
    std::ofstream outStream(outputPath, std::ios::out | std::ios::binary);
    fs::path pathFile(filePath);
    if (!fs::exists(pathFile)) {
        std::cerr << "[WARN] File not exists" << endl;
        return;
    }

    FileWriterUtil::writeCheck(outStream);
    FileWriterUtil::writeFreq(outStream, freq);
    if (fs::is_directory(filePath)) { //是目录
        FileWriterUtil::writeType(outStream,FileWriterUtil::TYPE_DIR);
        FileWriterUtil::writeDir(outStream, bitMap, filePath);
    } else { //是文件
        FileWriterUtil::writeType(outStream,FileWriterUtil::TYPE_FILE);
        FileWriterUtil::writeFile(outStream,bitMap,filePath);
    }
    outStream.close();
}

void ZipUtil::enCodeTest(const std::string& filePath, const std::string& outputPath) {
    namespace fs = std::filesystem;
    using std::cout;
    using std::endl;
    using std::cerr;
    std::vector<int> freq(256,0);
    calFileFreq(freq,filePath);
    HuffmanTree huffTree(freq);
    std::map<unsigned char, std::vector<bool>> bitMap = huffTree.getBitMap();
    std::ofstream outStream(outputPath, std::ios::out | std::ios::binary);
    fs::path pathFile(filePath);
    if (!fs::exists(pathFile)) {
        std::cerr << "[WARN] File not exists" << endl;
        return;
    }

    FileWriterUtil::writeCheck(outStream);
    FileWriterUtil::writeFreq(outStream, freq);
    if (fs::is_directory(filePath)) { //是目录
        FileWriterUtil::writeType(outStream,FileWriterUtil::TYPE_DIR);
        FileWriterUtil::writeDir(outStream, bitMap, filePath);
    } else { //是文件
        FileWriterUtil::writeType(outStream,FileWriterUtil::TYPE_FILE);
        FileWriterUtil::writeFile(outStream,bitMap,filePath);
    }
    outStream.close();

}
