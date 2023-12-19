#ifndef ZCODEWID_H
#define ZCODEWID_H

#include <QWidget>
#include "barcodeitem.h"
#include <QGraphicsView>

enum BarCode{
    Barcode_128=0,
    Barcode_39,
    EAN13,
    QRCode,
    Datamatrix,
    Pdf147,
};

class ZcodeWid : public QWidget
{
    Q_OBJECT
public:
    explicit ZcodeWid(QWidget *parent = nullptr);
    void createCode(const QString &str, int sym=QRCode);

signals:

protected:
    void setSymbol(int sym);

private:
    int m_symbol;
    BarcodeItem *mBarcode;
    QGraphicsView *mView;
};

#endif // ZCODEWID_H
