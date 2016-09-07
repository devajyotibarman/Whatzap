#include "whatzapwindow.h"

whatzapWindow::whatzapWindow() : QObject()
{
    view = new QWebEngineView();
    webSettings = view->settings();
    webSettings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    view->setUrl(QUrl(QStringLiteral("https://web.whatsapp.com")));
    connect(view->page(),
               SIGNAL(featurePermissionRequested(const QUrl&, QWebEnginePage::Feature)),
               SLOT(featurePermissionRequested(const QUrl&, QWebEnginePage::Feature)));
    connect(view->page()->profile(), SIGNAL(downloadRequested(QWebEngineDownloadItem*)),
                    this, SLOT(downloadRequested(QWebEngineDownloadItem*)));
    view->resize(800, 600);

    view->show();
    view->page()->setFeaturePermission(view->page()->url(), QWebEnginePage::MediaAudioVideoCapture, QWebEnginePage::PermissionGrantedByUser);
    view->page()->setFeaturePermission(view->page()->url(), QWebEnginePage::MediaAudioCapture, QWebEnginePage::PermissionGrantedByUser);
    view->page()->setFeaturePermission(view->page()->url(), QWebEnginePage::MediaVideoCapture, QWebEnginePage::PermissionGrantedByUser);
}

void whatzapWindow::featurePermissionRequested(const QUrl & securityOrigin,
    QWebEnginePage::Feature feature)
{

    // grant permission
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
    //Saves to ~/Downloads!
    download->accept();
}
