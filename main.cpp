#include <QApplication>
#include "mainwindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setFixedSize(1024, 720);
    mainWindow.show();

    return QApplication::exec();
}
