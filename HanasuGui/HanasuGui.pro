TEMPLATE = app

QT += qml quick quickcontrols2
CONFIG += c++11



fonts.path    = $$OUT_PWD/fonts
fonts.files   += *.ttf
INSTALLS       += fonts


SOURCES += main.cpp \
    controler.cpp \
    conversations.cpp \
    messages.cpp \
    friendsmodel.cpp

RESOURCES += qml.qrc

LIBS += -L/home/oskar/Programowanie/Hanasu/lib/ -lhanasu
DEPENDPATH += -L/home/oskar/Programowanie/Hanasu/lib/
INCLUDEPATH += /home/oskar/Programowanie/Hanasu/HanasuCommon/include/

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


HEADERS += \
    controler.h \
    conversations.h \
    messages.h \
    friendsmodel.h
