QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH +=/usr/local/pdfium/include
LIBS += -L/usr/local/pdfium/lib -lpdfium

equals (QMAKE_CXX, em++) {
    LIBS += -s USE_ZLIB=1 -s USE_LIBJPEG=1
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES = image.qrc

QMAKE_WASM_PTHREAD_POOL_SIZE = 12

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    pdfium_doc.cpp \
    pdfium_util.cpp \
    label_clickable.cpp \
    image_selection_group_widget.cpp \
    image_selection_widget.cpp


HEADERS += \
    mainwindow.h \
    pdfium_doc.h \
    pdfium_util.h \
    label_clickable.h \
    image_selection_group_widget.h \
    image_selection_widget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
