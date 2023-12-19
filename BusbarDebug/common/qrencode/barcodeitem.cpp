#include <QDebug>
#include "barcodeitem.h"

BarcodeItem::BarcodeItem(QGraphicsItem *parent)
        : QGraphicsItem(parent)
{
    //setRect(QRectF(0,0,200,200));
    barcodeItemPara.m_symbol = BARCODE_QRCODE;
    barcodeItemPara.m_hidetext = false;
    barcodeItemPara.m_text = "6922162891569";
    barcodeItemPara.m_height = 38;
    barcodeItemPara.m_scale = 8.0;
    barcodeItemPara.m_dot_size = 4.0 / 5.0;
    barcodeItemPara.m_pdf417CodeWords = 928;
    barcodeItemPara.m_securityLevel = -1;
    barcodeItemPara.m_input_mode = UNICODE_MODE;
    barcodeItemPara.m_option_3 = 0;
    barcodeItemPara.m_width = 0;
    barcodeItemPara.m_borderWidth = 1;
}

BarcodeItem::~BarcodeItem()
{
}

void BarcodeItem::setBoundingRect(QSize s)
{
    m_w = s.width()-5;
    m_h = s.height()-5;
}

QRectF BarcodeItem::boundingRect() const
{
    return QRectF(0, 0,m_w, m_h);
    if(m_boundingRect.width() == 0){
        m_boundingRect = QRectF(0,0,100,100);
    }

    return m_boundingRect;
}

void BarcodeItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    barcode.render(*painter,boundingRect(),ar);
}

QRectF BarcodeItem::rect() const
{
    return m_rect;
}

void BarcodeItem::setRect(const QRectF &rect)
{
    if(m_rect == rect)
        return;
    prepareGeometryChange();
    m_rect = rect;
    m_boundingRect = m_rect;
    update();
}

void BarcodeItem::drawBarcodeItem(BarcodeItem::BarcodeItemProp barcodeItemPara)
{
    barcode.setHeight(barcodeItemPara.m_height);
    barcode.setText(barcodeItemPara.m_text);
    barcode.setScale(barcodeItemPara.m_scale);
    barcode.setInputMode(barcodeItemPara.m_input_mode);
    //barcode.setBorderWidth(barcodeItemPara.m_width);
    barcode.setOption3(barcodeItemPara.m_option_3);
    // barcode.setHideText(barcodeItemPara.m_hidetext);
    barcode.setDotSize(barcodeItemPara.m_dot_size);
    //barcode.setPdf417CodeWords(barcodeItemPara.m_pdf417CodeWords);
    barcode.setSymbol(barcodeItemPara.m_symbol);
    //barcode.setSecurityLevel(barcodeItemPara.m_securityLevel);
    barcode.setBorderWidth(barcodeItemPara.m_borderWidth);
    barcode.setBorderType(Zint::QZint::IgnoreAspectRatio);

    this->setRect(QRectF(0,0,barcodeItemPara.m_width*barcodeItemPara.m_scale,barcode.height()*barcodeItemPara.m_scale));
    update();
}
