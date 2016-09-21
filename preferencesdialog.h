#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QStandardPaths>

namespace Ui {
class preferencesDialog;
}

class preferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit preferencesDialog(QWidget *parent = 0);
    ~preferencesDialog();

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    Ui::preferencesDialog *ui;
    QFile *preferencesFile;
    int lastWindowWidth;
    int lastWindowHeight;
    int lastWindowXpos;
    int lastWindowYpos;
    QString downloadPath;
    bool settingClose;
    bool settingMinimize;
};

#endif // PREFERENCESDIALOG_H
