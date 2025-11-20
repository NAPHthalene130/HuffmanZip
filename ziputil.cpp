#include "ziputil.h"
#include "huffmantree.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include "FileReaderUtil.h"
#include "FileWriterUtil.h"

// 引入 MainWindow 头文件以使用其方法
#include "mainwindow.h"

ZipUtil::ZipUtil() {}
ZipUtil::~ZipUtil() {}

unsigned ZipUtil::filesCountInDir(const std::filesystem::path& path) {
    unsigned int file_count = 0;
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (entry.is_regular_file()) {
                file_count++;
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "文件系统错误: " << e.what() << std::endl;
        std::cerr << "路径: " << path << std::endl;
    }
    return file_count;
}
void ZipUtil::calFileFreq(std::vector<int> &freq, std::string filePath)
{
    namespace fs = std::filesystem;
    int BUFFER_SIZE = 1024;
    std::fstream inputFile(filePath, std::ios::in | std::ios::binary);
    fs::path p(filePath);
    if (!fs::exists(p)) {
        std::cerr << "[ERROR][ZipUtil-calFileFreq-1]: Path is not existing" << filePath << std::endl;
    }
    if (fs::is_regular_file(p)) {
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
        try {
            for (const auto& entry : fs::directory_iterator(p)) {
                calFileFreq(freq, entry.path().string());
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "[ERROR][ZipUtil-calFileFreq-2]: Traveling files error" << e.what() << std::endl;
        }
    }
}

ZipUtil &ZipUtil::getInstance()
{
    static ZipUtil instance;
    return instance;
}

void ZipUtil::deCode(const std::string& filePath, const std::string& outputPath, MainWindow* window)
{
    namespace fs = std::filesystem;
    window->logWrite("开始解压...");
    fs::path p(filePath);
    if (std::filesystem::is_directory(p)) {
        window->setBarTotal(1);
    } else {
        window->setBarTotal(1);
    }
    namespace fs = std::filesystem;
    using std::cout;
    using std::endl;
    using std::cerr;

    // ... 原有逻辑保持不变 ...
    std::ifstream infile(filePath,std::ios::in | std::ios::binary);
    bool check = FileReaderUtil::readCheck(infile);

    if (!check) {
        cerr << "[WARN] CHECK FAIL" << endl;
        window->logWrite("校验失败");
        window->setBarState(2); // 失败红条
        window->setBarTotal(1);
        window->addBarProgress();
        return;
    } else {
        cout << "CHECK OK" << endl;
        window->logWrite("校验通过");
        window->addBarProgress();
        window->setBarState(3);
    }

    std::vector<HuffmanNode> tree = FileReaderUtil::readTree(infile);
    HuffmanTree huffmanTree(tree);
    std::map<unsigned char, std::vector<bool>> bitMap = huffmanTree.getBitMap();
    std::map<std::vector<bool>, unsigned char> reverseBitMap;
    for (const auto& pair : bitMap) {
        reverseBitMap[pair.second] = pair.first;
    }
    int type = FileReaderUtil::readType(infile);
    if (type == 0) {
        cout << "TYPE: DIR" << endl;
        int ope = FileReaderUtil::readOpe(infile);
        cout << "ope: " << ope << endl;
        FileReaderUtil::readDir(infile,reverseBitMap,outputPath, window);
    } else {
        cout << "TYPE: FILE" << endl;
        int ope = FileReaderUtil::readOpe(infile);
        cout << "ope: " << ope << endl;
        FileReaderUtil::readFileAndWrite(infile,reverseBitMap,outputPath, window);
    }
    int ope = FileReaderUtil::readOpe(infile);
    if (ope == 3) {
        cout << "DONE!" << endl;
        window->logWrite("解压完成");
        window->setBarTotal(1);
        window->addBarProgress();
        window->setBarState(0);
    } else {
        cout << "END WITH SOME WRONG" << endl;
        window->setBarTotal(1);
        window->addBarProgress();
        TODO: window->setBarState(1);
    }
    infile.close();
}

void ZipUtil::deCodeTest(const std::string& filePath, const std::string& outputPath) {
    // 测试代码保持不变或自行修改
    ZipUtil::deCode(filePath, outputPath, nullptr);
}

void ZipUtil::enCode(const std::string& filePath, const std::string& outputPath, MainWindow* window)
{
    window->logWrite("开始压缩...");
    // TODO: 预估文件数量或大小，调用 window->setBarTotal(count);

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
        window->logWrite("文件不存在");
        window->setBarState(2);
        window->setBarTotal(1);
        window->addBarProgress();
        return;
    }

    FileWriterUtil::writeCheck(outStream);
    FileWriterUtil::writeTree(outStream, huffTree.getTree());
    if (fs::is_directory(filePath)) {
        FileWriterUtil::writeType(outStream,FileWriterUtil::TYPE_DIR);
        FileWriterUtil::writeDir(outStream, bitMap, filePath,window);
    } else {
        FileWriterUtil::writeType(outStream,FileWriterUtil::TYPE_FILE);
        FileWriterUtil::writeFile(outStream,bitMap,filePath,window);
    }
    FileWriterUtil::writeOpe(outStream,FileWriterUtil::OPE_END);
    outStream.close();
    window->logWrite("压缩完成");
    window->setBarState(0);
    window->setBarTotal(1);
    window->addBarProgress();
}

void ZipUtil::enCodeTest(const std::string& filePath, const std::string& outputPath) {
     // 测试代码保持不变或自行修改
    ZipUtil::enCode(filePath, outputPath, nullptr);
}