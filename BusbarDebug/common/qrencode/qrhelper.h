/*
    https://fukuchi.org/works/qrencode/manual/index.html
*/

#ifndef QRHELPER_H
#define QRHELPER_H
#include <QtCore>
#include <QObject>
#include <QPixmap>
#include <QFrame>

extern "C"{
    #include "libqrencode/qrencode.h"
}

class QRHelper : public QObject
{
    Q_OBJECT
public:
    explicit QRHelper(QObject *parent = 0);

    QPixmap generateQR(QString strContent, int width = 300, int height = 300);
signals:

public slots:


private:

};

#endif // QRHELPER_H
