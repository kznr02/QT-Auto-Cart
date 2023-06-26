QT       += core gui multimedia multimediawidgets network sql serialport

INCLUDEPATH += /opt/poky/3.1.5/sysroots/aarch64-poky-linux/usr/include/opencv4/ \
                /opt/poky/3.1.5/sysroots/aarch64-poky-linux/usr/include/opencv2/ \

include(QZXing/QZXing.pri)

#LIBS += /usr/local/lib/libopencv* \
LIBS += /opt/poky/3.1.5/sysroots/aarch64-poky-linux/usr/lib64/libopencv* \


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    http.cpp \
    main.cpp \
    map.cpp \
    scan.cpp \
    shop.cpp \
    speech.cpp \
    va.cpp \
    widget.cpp

HEADERS += \
    http.h \
    map.h \
    scan.h \
    shop.h \
    speech.h \
    va.h \
    widget.h

FORMS += \
    map.ui \
    scan.ui \
    shop.ui \
    va.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc
