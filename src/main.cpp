#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion (APP_VERSION);
    a.setApplicationName ("DoujinFix");

    QTranslator translator;
    translator.load("zh_CN.qm");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
