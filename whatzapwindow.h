#ifndef WHATZAPWINDOW_H
#define WHATZAPWINDOW_H

#include <QObject>
#include <QWidget>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QWebEngineProfile>
#include <QWebEngineDownloadItem>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QDebug>
#include <QStandardPaths>
#include <QFile>
#include <QEvent>
#include <QTextStream>
#include <QMenu>
#include <QMessageBox>
#include <QLockFile>

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

class whatzapWindow:public QObject
{
    Q_OBJECT

public:
    whatzapWindow();
    ~whatzapWindow();
private:
    QWebEngineView *view;
    QWebEngineView *secondView;
    QWebEngineSettings *webSettings;
    QSystemTrayIcon *trayIcon;
    QFile *iniFile;
    int lastWindowWidth;
    int lastWindowHeight;
    int lastWindowXpos;
    int lastWindowYpos;
    QString downloadPath;
    bool settingClose;
    bool settingMinimize;
    QMenu *trayMenu;
    QAction *Settings;
    QAction *Reload;
    QAction *Show_Hide;
    QAction *About;
    QAction *Exit;
    QLockFile *lockFile;
private slots:
    void featurePermissionRequested(const QUrl & securityOrigin,
        QWebEnginePage::Feature feature);
    void downloadRequested(QWebEngineDownloadItem* download);
    void showDownloadFinished();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void openSettingsDialog();
    void reloadWindow();
    void openAboutDialog();
    void windowShowHide();
    void quitProgram();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // WHATZAPWINDOW_H
