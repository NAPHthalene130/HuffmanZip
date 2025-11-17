#ifndef ZIPUTIL_H
#define ZIPUTIL_H
#include <string>
#include <vector>
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

    static ZipUtil& getInstance();
    static void deCode(const std::string& filePath, const std::string& outputPath);
    static void deCodeTest(const std::string& filePath, const std::string& outputPath);
    static void enCode(const std::string& filePath, const std::string& outputPath);
    static void enCodeTest(const std::string& filePath, const std::string& outputPath);
};

#endif // ZIPUTIL_H
