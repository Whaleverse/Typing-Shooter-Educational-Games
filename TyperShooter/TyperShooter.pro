#-------------------------------------------------
#
# Project created by QtCreator 2018-11-20T22:06:08
#
#-------------------------------------------------

QT       += core gui
QT       += widgets
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TyperShooter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    src/bullet.cpp \
    src/game.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/missile.cpp \
    src/shootablelabel.cpp \
    src/typerwordgenerator.cpp \
    WordGenerator/src/partitioner.cpp \
    WordGenerator/src/strictpartitioner.cpp \
    WordGenerator/src/textreader.cpp \
    WordGenerator/src/wordgenerator.cpp \

HEADERS += \
    include/bullet.h \
    include/game.h \
    include/mainwindow.h \
    include/missile.h \
    include/shootablelabel.h \
    include/typerwordgenerator.h \
    WordGenerator/include/partitioner.h \
    WordGenerator/include/strictpartitioner.h \
    WordGenerator/include/textreader.h \
    WordGenerator/include/wordgenerator.h

INCLUDEPATH += $$PWD/include

FORMS += \
        mainwindow.ui

macx {
    LIBS += -L"/usr/local/lib"

    CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window
    CONFIG(debug, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-system -lsfml-network -lsfml-window

    INCLUDEPATH += "/usr/local/include"
    DEPENDPATH += "/usr/local/include"
}
LIBS += -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -L$$PWD/../Box2D/Build/bin/x86_64/Debug/ -lBox2D

INCLUDEPATH += $$PWD/../Box2D/
INCLUDEPATH += $$PWD/../Box2D/Testbed/
DEPENDPATH += $$PWD/../Box2D/
DEPENDPATH += $$PWD/../Box2D/Testbed/

PRE_TARGETDEPS += $$PWD/../Box2D/Build/bin/x86_64/Debug/libBox2D.a

INCLUDEPATH += $$PWD/WordGenerator/include

DISTFILES += \
    resources/Fonts/Minecraftia.ttf \
    resources/Fonts/Zpix.ttf \
    resources/Languages/cedict.gb \
    resources/premake/premake5 \
    resources/Images/bullet.png \
    resources/Images/explosion.png \
    resources/Images/menuBackground.png \
    resources/Images/missileship.png \
    resources/Images/missileSpriteSheet.png \
    resources/Images/missileSpriteSheetRotated.png \
    resources/Images/slcBaseDayTall.png \
    resources/Images/slcBaseEveningTall.png \
    resources/Images/slcBaseNightTall.png \
    resources/Images/ui.png \
    resources/Images/pause.svg \
    resources/Images/play.svg \
    resources/Languages/note.md \
    resources/Images/pixelart/slcBaseDay.pixil \
    resources/Images/pixelart/slcBaseDayTall.pixil \
    resources/Images/pixelart/slcBaseEvening.pixil \
    resources/Images/pixelart/slcBaseEveningTall.pixil \
    resources/Images/pixelart/slcBaseNight.pixil \
    resources/Images/pixelart/slcBaseNightTall.pixil \
    resources/Languages/cedict_readme.txt \
    resources/Languages/chinese.txt \
    resources/Languages/Chinese_readme.txt \
    resources/Languages/english.txt \
    resources/Languages/spanish.txt

RESOURCES += \
    resource.qrc
