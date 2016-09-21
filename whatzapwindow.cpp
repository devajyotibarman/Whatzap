#include "whatzapwindow.h"

whatzapWindow::whatzapWindow() : QObject()
{
    //Main Interface
    view = new QWebEngineView();
    webSettings = view->settings();
    webSettings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    view->setUrl(QUrl(QStringLiteral("https://web.whatsapp.com")));

    //Setup Tray Icon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/icon.png"));
    trayIcon->show(); //TODO: Add a menu to Systray Right Click, Left Click to show/hide

    //Setup Event Handlers for Page
    connect(view->page(),
               SIGNAL(featurePermissionRequested(const QUrl&, QWebEnginePage::Feature)),
               SLOT(featurePermissionRequested(const QUrl&, QWebEnginePage::Feature)));
    connect(view->page()->profile(), SIGNAL(downloadRequested(QWebEngineDownloadItem*)),
                    this, SLOT(downloadRequested(QWebEngineDownloadItem*)));

    //Setup Window
    view->resize(800, 600); //TODO: Remember Window Size and Position
    view->setWindowIcon(QIcon(":/icons/icon.png"));
    view->show();
}

void whatzapWindow::featurePermissionRequested(const QUrl & securityOrigin,
                                               QWebEnginePage::Feature feature)
{
    // Grant permission
    switch (feature) {
    case QWebEnginePage::MediaAudioCapture:
    case QWebEnginePage::MediaVideoCapture:
    case QWebEnginePage::Notifications:
    case QWebEnginePage::MediaAudioVideoCapture:
            view->page()->setFeaturePermission(view->page()->url(),
                feature, QWebEnginePage::PermissionGrantedByUser);
        break;
    default:
        qDebug() << securityOrigin << feature;
    }
}

void whatzapWindow::downloadRequested(QWebEngineDownloadItem *download)
{
    //Saves to ~/Downloads! TODO: Set Location
    connect(download, SIGNAL(finished()), this, SLOT(showDownloadFinished()));
    download->accept();
}

void whatzapWindow::showDownloadFinished()
{
    qDebug() << "Downloaded";
    trayIcon->showMessage("Downloaded Completed", "Download Finished");
}
