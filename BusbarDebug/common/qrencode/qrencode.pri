include(libqrencode/libqrencode.pri)
include(backend/backend.pri)
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/barcodeitem.h \
    $$PWD/qrhelper.h \
    $$PWD/qrwidget.h \
    $$PWD/zcodewid.h


SOURCES += \
    $$PWD/barcodeitem.cpp \
    $$PWD/qrhelper.cpp \
    $$PWD/qrwidget.cpp \
    $$PWD/zcodewid.cpp


FORMS +=



