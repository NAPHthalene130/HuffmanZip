#include "mainwindow.h"
#include "ziputil.h"
#include <QApplication>
#include <QDebug>
#include <string>
#include <filesystem>
#include <iostream>
int main(int argc, char *argv[])
{
    namespace fs = std::filesystem;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug() << "Hello World!";
    fs::path testPath = "E:\\Steam\\steamapps\\common\\Hearts of Iron IV\\hoi4.exe";
    const std::string testOutPath = "E:\\Steam\\steamapps\\common\\Hearts of Iron IV\\hoi4.exe.huff";
    std::string inputPath = testPath.string();
    ZipUtil::enCodeTest(inputPath, testOutPath);
    ZipUtil::deCodeTest(testOutPath,inputPath);
    return a.exec();
}
