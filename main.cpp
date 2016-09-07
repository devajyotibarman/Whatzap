#include <QApplication>
#include <QWebEngineView>
#include "whatzapwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    whatzapWindow mainWindow;

    return app.exec();
}

