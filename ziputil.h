#ifndef ZIPUTIL_H
#define ZIPUTIL_H
#include <filesystem>
#include <string>
#include <vector>

class MainWindow;

class ZipUtil
{
private:
    ZipUtil();
    ~ZipUtil();
    static void calFileFreq(std::vector<int> &freq, std::string filePath);
public:
    // 禁用拷贝构造函数
    ZipUtil(const ZipUtil&) = delete;
    // 禁用赋值操作符
    ZipUtil& operator=(const ZipUtil&) = delete;
    static unsigned int filesCountInDir(const std::filesystem::path& path);
    static ZipUtil& getInstance();

    static void deCode(const std::string& filePath, const std::string& outputPath, MainWindow* window = nullptr);
    static void deCodeTest(const std::string& filePath, const std::string& outputPath);

    static void enCode(const std::string& filePath, const std::string& outputPath, MainWindow* window = nullptr);
    static void enCodeTest(const std::string& filePath, const std::string& outputPath);
};

#endif // ZIPUTIL_H
