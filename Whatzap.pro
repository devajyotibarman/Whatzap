TEMPLATE = app

QT += webenginewidgets

SOURCES += main.cpp \
    whatzapwindow.cpp \
    aboutdialog.cpp \
    preferencesdialog.cpp
TARGET = Whatzap

HEADERS += \
    whatzapwindow.h \
    aboutdialog.h \
    preferencesdialog.h

RESOURCES += \
    whatzap.qrc

FORMS += \
    aboutdialog.ui \
    preferencesdialog.ui
