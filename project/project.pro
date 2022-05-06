QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = editor
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/activationelement.cpp \
    src/actorelement.cpp \
    src/classelement.cpp \
    src/itemobject.cpp \
    src/main.cpp \
    src/editor_ui.cpp \
    src/messageanchor.cpp \
    src/objectelement.cpp \
    src/projectmanager.cpp \
    src/seqmessage.cpp \
    src/workscene.cpp \
    src/workview.cpp

HEADERS += \
    src/headers/actorelement.h \
    src/headers/classelement.h \
    src/headers/classlines.h \
    src/headers/editor_ui.h \
    src/headers/itemobject.h \
    src/headers/objectelement.h \
    src/headers/projectmanager.h \
    src/headers/workscene.h \
    src/headers/workview.h

FORMS += \
    src/gui/actorelement.ui \
    src/gui/classelement.ui \
    src/gui/editor_ui.ui \
    src/gui/itemobject.ui \
    src/gui/messageanchor.ui \
    src/gui/objectelement.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
