QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += debug

TARGET = editor_ui
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    classelement.cpp \
    itemobject.cpp \
    main.cpp \
    editor_ui.cpp \
    messageanchor.cpp \
    objectelement.cpp \
    projectmanager.cpp \
    seqmessage.cpp \
    workscene.cpp \
    workview.cpp

HEADERS += \
    classelement.h \
    classlines.h \
    editor_ui.h \
    itemobject.h \
    objectelement.h \
    projectmanager.h \
    workscene.h \
    workview.h

FORMS += \
    classelement.ui \
    editor_ui.ui \
    itemobject.ui \
    messageanchor.ui \
    objectelement.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
