#ifndef WHATZAPWINDOW_H
#define WHATZAPWINDOW_H

#include <QObject>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QWebEngineDownloadItem>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QDebug>

class whatzapWindow:public QObject
{
    Q_OBJECT

public:
    whatzapWindow();
private:
    QWebEngineView *view;
    QWebEngineView *secondView;
    QWebEngineSettings *webSettings;
    QSystemTrayIcon *trayIcon;
private slots:
    void featurePermissionRequested(const QUrl & securityOrigin,
        QWebEnginePage::Feature feature);
    void downloadRequested(QWebEngineDownloadItem* download);
    void showDownloadFinished();
};

#endif // WHATZAPWINDOW_H
