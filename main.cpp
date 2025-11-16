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
    fs::path testPath = "H:/CODE/CPP/HuffmanZip/TEST";
    std::string inputPath = testPath.string();
    ZipUtil::enCodeTest(inputPath);
    return a.exec();
}
