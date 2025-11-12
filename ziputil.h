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
    static void deCode(std::string filePath);
    static void enCode(std::string filePath);
};

#endif // ZIPUTIL_H
