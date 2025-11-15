#include "mainwindow.h"
#include "ziputil.h"
#include <QApplication>
#include <string>
#include <filesystem>
int main(int argc, char *argv[])
{
    namespace fs = std::filesystem;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    fs::path testPath = "C:/Users/NAPH/Desktop/QT/HuffmanZip/TEST";
    std::string inputPath = testPath.string();
    ZipUtil::enCode(inputPath);
    return a.exec();
}
