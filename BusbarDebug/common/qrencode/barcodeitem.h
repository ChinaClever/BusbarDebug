#ifndef BARCODEITEM_H
#define BARCODEITEM_H

#include <QGraphicsItem>
#include "backend/qzint.h"

class BarcodeItem : public QGraphicsItem
{
public:
    explicit BarcodeItem(QGraphicsItem *parent = Q_NULLPTR);
    ~BarcodeItem();

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

public:
    mutable Zint::QZint barcode;

    Zint::QZint::AspectRatioMode ar;

    QRectF rect() const;
    void setRect(const QRectF &rect);
    inline void setRect(qreal x, qreal y, qreal w, qreal h);

    struct BarcodeItemProp{
        int m_symbol;
        QString m_text;
        QString m_primaryMessage;
        int m_height;
        int m_borderWidth;
        int m_width;
        int m_securityLevel;
        int m_pdf417CodeWords;
        int m_input_mode;
        QColor m_fgColor;
        QColor m_bgColor;
        QString m_lastError;
        int m_error;
        int m_whitespace;
        zint_symbol * m_zintSymbol;
        float m_scale;
        int m_option_3;
        bool m_hidetext;
        float m_dot_size;
    } barcodeItemPara;

    void drawBarcodeItem(BarcodeItemProp barcodeItemPara);
    void setBoundingRect(QSize s);

private:
    int m_h, m_w;
    QRectF m_rect;
    mutable QRectF m_boundingRect;
};

inline void BarcodeItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    setRect(QRectF(x, y, w, h));
}


#endif // BARCODEITEM_H
