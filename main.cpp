#include <QApplication>
#include <QWebEngineView>
#include "whatzapwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);
    whatzapWindow mainWindow;

    return app.exec();
}

