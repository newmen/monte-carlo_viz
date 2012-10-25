#include <QApplication>
#include <cstdlib>
#include <ctime>
#include "mainwindow_context.h"

int main(int argc, char *argv[]) {
    srand(time(0));

    QApplication app(argc, argv);

    MainWindowContext mainWindow;
    mainWindow.show();

    return app.exec();
}
