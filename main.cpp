#include "mainwindow.h"
#include "ziputil.h"
#include <QApplication>
#include <string>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    std::string inputPath = "H:/CODE/QT/HuffmanZip/TEST.png";
    ZipUtil::enCode(inputPath);
    return a.exec();
}
