#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

preferencesDialog::preferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::preferencesDialog)
{
    //Setup View From INI File
    preferencesFile = new QFile(QStandardPaths::locate(QStandardPaths::HomeLocation,
                                               QString(),
                                               QStandardPaths::LocateDirectory).append(".whatzapp.ini"));
    if (!preferencesFile->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "Failed to Open File"<< preferencesFile->errorString();
    }

    QTextStream in(preferencesFile);

    //Read Initialization Parameters
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
    preferencesFile->close();

    ui->setupUi(this);

    //Initialize Download Location Preference
    ui->lineEdit->setText(downloadPath);

    //Initialize "Ask before Close" Option
    if(settingClose == true)
    {
        ui->checkBox->setCheckState(Qt::Checked);
    }
    else
    {
        ui->checkBox->setCheckState(Qt::Unchecked);
    }

    //Initialize "Minimize on Close" Option
    if(settingMinimize == true)
    {
        ui->checkBox_2->setCheckState(Qt::Checked);
    }
    else
    {
        ui->checkBox_2->setCheckState(Qt::Unchecked);
    }
}

preferencesDialog::~preferencesDialog()
{
    delete ui;
}

void preferencesDialog::on_buttonBox_accepted()
{
//    qDebug() << "Accepted";
    if (!preferencesFile->open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        qDebug() << "Failed to Open File"<< preferencesFile->errorString();
    }

    preferencesFile->reset();
    QTextStream out(preferencesFile);

    //Write Initialization Parameters
    out << lastWindowWidth << endl;
    out << lastWindowHeight << endl;
    out << lastWindowXpos << endl;
    out << lastWindowYpos << endl;
    out << downloadPath << endl;

    if(ui->checkBox->checkState() == Qt::Checked)
    {
        settingClose = true;
    }
    else
    {
        settingClose = false;
    }

    out << settingClose << endl;

    if(ui->checkBox_2->checkState() == Qt::Checked)
    {
        settingMinimize = true;
    }
    else
    {
        settingMinimize = false;
    }

    out << settingMinimize << endl;

//    qDebug() << lastWindowWidth;
//    qDebug() << lastWindowHeight;
//    qDebug() << lastWindowXpos;
//    qDebug() << lastWindowYpos;
//    qDebug() << downloadPath;
//    qDebug() << settingClose;
//    qDebug() << settingMinimize;

    preferencesFile->close();
}

void preferencesDialog::on_pushButton_clicked()
{
    //Update Download Location
    downloadPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    downloadPath,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
//    qDebug() << "Download Directory:" << downloadPath;
    ui->lineEdit->setText(downloadPath);
}
