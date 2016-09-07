#ifndef WHATZAPWINDOW_H
#define WHATZAPWINDOW_H

#include <QObject>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QWebEngineDownloadItem>

class whatzapWindow:public QObject
{
    Q_OBJECT

public:
    whatzapWindow();
private:
    QWebEngineView *view;
    QWebEngineSettings *webSettings;
private slots:
    void featurePermissionRequested(const QUrl & securityOrigin,
        QWebEnginePage::Feature feature);
    void downloadRequested(QWebEngineDownloadItem* download);
};

#endif // WHATZAPWINDOW_H
