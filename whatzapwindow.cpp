#include "whatzapwindow.h"

whatzapWindow::whatzapWindow() : QWebEngineView()
{
    //Check For Running Processes
    lockFile = new QLockFile(QStandardPaths::locate(QStandardPaths::HomeLocation,
                                                    QString(),
                                                    QStandardPaths::LocateDirectory).append(".whatzap.lock"));
    if(lockFile->tryLock(0) == false)
    {
        qDebug() << "Could not acquire File Lock";
        exit(0);
    }

    //Initialize Default Parameters
    lastWindowWidth = 800;
    lastWindowHeight = 600;
    lastWindowXpos = 0;
    lastWindowYpos = 0;
    downloadPath = QStandardPaths::locate(QStandardPaths::DownloadLocation,
                               QString(), QStandardPaths::LocateDirectory);
    downloadPath.append("Whatsapp"); //Add New Whatsapp Folder
    settingClose = true;
    settingMinimize = true;

    //Setup View From INI File
    iniFile = new QFile(QStandardPaths::locate(QStandardPaths::HomeLocation,
                                               QString(),
                                               QStandardPaths::LocateDirectory).append(".whatzapp.ini"));
    if(!iniFile->exists())
    {
        // File Does Not Exist, Create New
        if (!iniFile->open(QIODevice::ReadWrite| QIODevice::Text))
        {
            qDebug() << "Failed to Open File" << iniFile->errorString();
            exit(31);
        }
        iniFile->reset();
        QTextStream out(iniFile);
        out << lastWindowWidth << endl;
        out << lastWindowHeight << endl;
        out << lastWindowXpos << endl;
        out << lastWindowYpos << endl;
        out << downloadPath << endl;
        out << settingClose << endl;
        out << settingMinimize << endl;
        iniFile->close();
    }

    //File Found. TODO Make sure file is not Corrupt
    if (!iniFile->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Failed to Open File"<< iniFile->errorString();
    }

    QTextStream in(iniFile);

    //Initialize Parameters from file
    QString line = in.readLine();
    lastWindowWidth = line.toInt();
//    qDebug() << lastWindowWidth;
    line = in.readLine();
    lastWindowHeight = line.toInt();
//    qDebug() << lastWindowHeight;
    line = in.readLine();
    lastWindowXpos = line.toInt();
//    qDebug() << lastWindowXpos;
    line = in.readLine();
    lastWindowYpos = line.toInt();
//    qDebug() << lastWindowYpos;
    line = in.readLine();
    downloadPath = line;
//    qDebug() << downloadPath;
    line = in.readLine();
    settingClose = (line.toInt() ? true : false);
//    qDebug() << settingClose;
    line = in.readLine();
    settingMinimize = (line.toInt() ? true : false);
//    qDebug() << settingMinimize;
    iniFile->close();
}

whatzapWindow::~whatzapWindow()
{
}

void whatzapWindow::start()
{
    this->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

    //Setup URL
    this->setUrl(QUrl(QStringLiteral("https://web.whatsapp.com/")));

    //Setup Tray Menu Actions
    Settings = new QAction("Settings");
    Settings->setIcon(QIcon::fromTheme("emblem-system"));
    connect(Settings, SIGNAL(triggered(bool)), this, SLOT(openSettingsDialog()));

    Reload = new QAction("Reload Window");
    Reload->setIcon(QIcon::fromTheme("view-refresh"));
    connect(Reload, SIGNAL(triggered(bool)), this, SLOT(reloadWindow()));

    Show_Hide = new QAction("Show/Hide");
    Show_Hide->setIcon(QIcon::fromTheme("window-close"));
    connect(Show_Hide, SIGNAL(triggered(bool)), this, SLOT(windowShowHide()));

    About = new QAction("About");
    About->setIcon(QIcon::fromTheme("system-help"));
    connect(About, SIGNAL(triggered(bool)), this, SLOT(openAboutDialog()));

    Exit = new QAction("Exit");
    Exit->setIcon(QIcon::fromTheme("application-exit"));
    connect(Exit, SIGNAL(triggered(bool)), this, SLOT(quitProgram()));

    //Setup Tray Menu
    trayMenu = new QMenu();
    trayMenu->addAction(Settings);
    trayMenu->addAction(Reload);
    trayMenu->addAction(Show_Hide);
    trayMenu->addAction(About);
    trayMenu->addAction(Exit);

    //Setup Tray Icon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/icons/main-icon.svg"));
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->setContextMenu(trayMenu);

    //Setup Event Handlers for Page
    connect(this->page(),
            SIGNAL(featurePermissionRequested(const QUrl&, QWebEnginePage::Feature)),
            SLOT(featurePermissionRequested(const QUrl&, QWebEnginePage::Feature)));
    connect(this->page()->profile(), SIGNAL(downloadRequested(QWebEngineDownloadItem*)),
            this, SLOT(downloadRequested(QWebEngineDownloadItem*)));

    this->installEventFilter(this); //Get all Events, filter the ones we want in Event Handler


    //Setup Window
    this->resize(lastWindowWidth, lastWindowHeight);
    this->move(lastWindowXpos, lastWindowYpos);
    this->setWindowIcon(QIcon(":/icons/main-icon.svg"));
    this->show();
}

void whatzapWindow::featurePermissionRequested(const QUrl &securityOrigin,
                                               QWebEnginePage::Feature feature)
{
    // Grant permission
    switch (feature) {
    case QWebEnginePage::MediaAudioCapture:
    case QWebEnginePage::MediaVideoCapture:
    case QWebEnginePage::Notifications:
    case QWebEnginePage::MediaAudioVideoCapture:
        this->page()->setFeaturePermission(this->page()->url(),
                                           feature, QWebEnginePage::PermissionGrantedByUser);
        break;
    default:
        qDebug() << securityOrigin << feature;
    }
}

void whatzapWindow::downloadRequested(QWebEngineDownloadItem *download)
{
    connect(download, SIGNAL(finished()), this, SLOT(showDownloadFinished()));
    QString a = downloadPath;
    a.append(download->path().remove(1, download->path().lastIndexOf('/')));
    download->setPath(a);
//    qDebug() << a;
    download->accept();
}

void whatzapWindow::showDownloadFinished()
{
//    qDebug() << "Downloaded";
    trayIcon->showMessage("Downloaded Completed", "Download Finished");
}

bool whatzapWindow::eventFilter(QObject *obj, QEvent *event)
{
    int ret = QMessageBox::Ok;

    if(obj == this)
    {
        if (event->type() == QEvent::Resize)
        {
//            qDebug() << "Resized to" << this->width() << this->height();
            lastWindowWidth = this->width();
            lastWindowHeight = this->height();
        }

        if (event->type() == QEvent::Move)
        {
//            qDebug() << "Moved to" << this->x() << this->y();
            lastWindowXpos = this->x();
            lastWindowYpos = this->y();
        }

        if (event->type() == QEvent::Close)
        {
            if(settingClose == true)
            {
                QMessageBox msgBox;
                msgBox.setText("Do you want to close Whatzap");
                msgBox.setWindowIcon(QIcon(":/icons/main-icon.svg"));
                msgBox.setWindowTitle("Closing Whatzap");
                msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Cancel);
                ret = msgBox.exec();
            }
//            qDebug() << "Closed! Saving Preferences";
            if(ret == QMessageBox::Ok)
            {
//                qDebug() << "Closing! Saving Preferences";
                if (!iniFile->open(QIODevice::ReadWrite| QIODevice::Text | QIODevice::Truncate))
                {
                    qDebug() << "Failed to Open File" << iniFile->errorString();
                    exit(31);
                }
                QTextStream out(iniFile);
                iniFile->seek(0);

                //Write Window Size and Position before Closing
                out << lastWindowWidth << endl;
                out << lastWindowHeight << endl;
                out << lastWindowXpos << endl;
                out << lastWindowYpos << endl;
                out << downloadPath << endl;
                out << settingClose << endl;
                out << settingMinimize << endl;
                iniFile->close();
                lockFile->unlock();
            }
            else
            {
//                qDebug() << "Not Closing";
                event->setAccepted(false);
                return true;
            }
        }

        if (event->type() == QEvent::WindowStateChange)
        {
//            qDebug() << event;
            if(this->isMinimized())
            {
//                qDebug() << "Minimized";
                if(settingMinimize == true)
                {
                    this->hide();
                }
            }
            else if (this->isMaximized())
            {
//                qDebug() << "Maximized";
            }
        }
    }
    return false;
}

void whatzapWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        if(this->isHidden())
        {
            this->show();
        }
        else
        {
            this->hide();
        }
        break;
    default:
//        qDebug() << "Clicked" << reason;
        break;
    }
}

void whatzapWindow::openSettingsDialog()
{
//    qDebug() << "Open Settings";
    //Show Preferences Dialog
    preferencesDialog p;
    p.setWindowIcon(QIcon(":/icons/settings-icon.svg"));
    if(p.exec() == QDialog::Accepted)
    {
        if (!iniFile->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            qDebug() << "Failed to Open File" << iniFile->errorString();
            exit(31);
        }
        //Reinitialize Init Parameters Except for Window Size and Position
        QTextStream in(iniFile);
        QString line = in.readLine();
//        lastWindowWidth = line.toInt();
//        qDebug() << lastWindowWidth;
        line = in.readLine();
//        lastWindowHeight = line.toInt();
//        qDebug() << lastWindowHeight;
        line = in.readLine();
//        lastWindowXpos = line.toInt();
//        qDebug() << lastWindowXpos;
        line = in.readLine();
//        lastWindowYpos = line.toInt();
//        qDebug() << lastWindowYpos;
        line = in.readLine();
        downloadPath = line;
//        qDebug() << downloadPath;
        line = in.readLine();
        settingClose = (line.toInt() ? true : false);
//        qDebug() << settingClose;
        line = in.readLine();
        settingMinimize = (line.toInt() ? true : false);
//        qDebug() << settingMinimize;
        iniFile->close();
    }
}

void whatzapWindow::reloadWindow()
{
    this->reload();
}

void whatzapWindow::openAboutDialog()
{
//    qDebug() << "Open About";
    //Show About Dialog
    aboutDialog a;
    a.setWindowIcon(QIcon(":/icons/about-icon.svg"));
    a.exec();
}

void whatzapWindow::windowShowHide()
{
    if(this->isHidden())
    {
        this->show();
    }
    else
    {
        this->hide();
    }
}

void whatzapWindow::quitProgram()
{
    this->close();
}

QWebEngineView *whatzapWindow::createWindow(QWebEnginePage::WebWindowType type)
{
//    qDebug() << "Clicked";
    if(type == QWebEnginePage::WebBrowserWindow ||
       type == QWebEnginePage::WebBrowserTab ||
       type == QWebEnginePage::WebDialog ||
       type == QWebEnginePage::WebBrowserBackgroundTab)
    {
//        qDebug() << "Opening New Window" << type;
        QWebEngineView *secondView = new QWebEngineView();
        secondView->setAttribute(Qt::WA_DeleteOnClose, true);
        secondView->setWindowIcon(QIcon(":/icons/browser-icon.svg"));
        secondView->show();
        return secondView;
    }
    return NULL;
}
