#include "MainWindow.h"
#include <QGuiApplication>
#include <qapplication.h>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    return app.exec();
}
