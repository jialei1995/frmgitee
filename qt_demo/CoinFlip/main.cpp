#include "mainscene.h"
#include <QApplication>

#include "dataconfig.h"
#include <QDebug>

// 可以使用hm nis eidt软件把.exe文件打包发布

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScene w;
    w.show();

    return a.exec();
}
