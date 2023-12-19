#include "qrwidget.h"
#include <QPainter>
#include <QImage>
#include <QGridLayout>

QRWidget::QRWidget(QWidget *parent) : QWidget(parent)
{
    qr = NULL;
    //setString("Hello QR Code");
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

QRWidget::~QRWidget()
{
    if(qr != NULL)
    {
        QRcode_free(qr);
    }
}


int QRWidget::getQRWidth() const
{
    if(qr != NULL)
    {
        return qr->width;
    }
    else
    {
        return 0;
    }
}

/**
 * Version
   二维码一共有40个尺寸。官方叫版本Version。Version 1是21 x 21的矩阵，
    Version 2是 25 x 25的矩阵，Version 3是29的尺寸，每增加一个version，就会增加4的尺寸，
    公式是：(V-1)*4 + 21（V是版本号） 最高Version 40，(40-1)*4+21 = 177，所以最高是177 x 177 的正方形。
    // 21:Version=1,最大字符=17(8.5个汉字)
    // 25:Version=2,最大字符=32(16个汉字)
    // 29:Version=3,最大字符=49(24.5个汉字)
    // 33:Version=4,最大字符=78(39个汉字)
    // 37:Version=5,最大字符=106(53个汉字)
    // 41:Version=6,最大字符=134(67个汉字)
    // 45:Version=7,最大字符=154(77个汉字)
    // 49:Version=8,最大字符=192(96个汉字)

    Error Correction Code Level
    二维码中有四种级别的纠错，这就是为什么二维码有残缺还能扫出来，也就是为什么有人在二维码的中心位置加入图标。
    二维码有4种纠错等级，可恢复的码字比例为：
    #define QR_LEVEL_L 0 //7%的字码可被修正
    #define QR_LEVEL_M 1 //15%的字码可被修正
    #define QR_LEVEL_Q 2 //25%的字码可被修正
    #define QR_LEVEL_H 3 //30%的字码可被修正
 *
 */
void QRWidget::setString(QString str, int v)
{
    string = str;
    if(qr != NULL)
    {
        QRcode_free(qr);
    }
    qr = QRcode_encodeString(string.toStdString().c_str(),
                             v,
                             QR_ECLEVEL_Q,
                             QR_MODE_8,
                             1);
    update();
    this->adjustSize();
}

QSize QRWidget::sizeHint()  const
{
    QSize s;
    if(qr != NULL)
    {
        int qr_width = qr->width > 0 ? qr->width : 1;
        s = QSize(qr_width * 4, qr_width * 4);
    }
    else
    {
        s = QSize(50, 50);
    }
    return s;
}

QSize QRWidget::minimumSizeHint()  const
{
    QSize s;
    if(qr != NULL)
    {
        int qr_width = qr->width > 0 ? qr->width : 1;
        s = QSize(qr_width, qr_width);
    }
    else
    {
        s = QSize(50, 50);
    }
    return s;
}

bool QRWidget::saveImage(QString fileName)
{
    if(!fileName.isEmpty())
    {
        QSize s = minimumSizeHint();
        QImage image(s, QImage::Format_Mono);
        QPainter painter(&image);
        QColor background(Qt::white);
        painter.setBrush(background);
        painter.setPen(Qt::NoPen);
        painter.drawRect(0, 0, s.width(), s.height());
        if(qr != NULL)
        {
            draw(painter, s.width(), s.height());
        }
        return image.save(fileName);
    }
    else
    {
        return false;
    }
}

void QRWidget::draw(QPainter &painter, int width, int height)
{
    QColor foreground(Qt::black);
    painter.setBrush(foreground);
    const int qr_width = qr->width > 0 ? qr->width : 1;
    double scale_x = width / qr_width;
    double scale_y = height / qr_width;
    for( int y = 0; y < qr_width; y ++)
    {
        for(int x = 0; x < qr_width; x++)
        {
            unsigned char b = qr->data[y * qr_width + x];
            if(b & 0x01)
            {
                QRectF r(x * scale_x, y * scale_y, scale_x, scale_y);
                painter.drawRects(&r, 1);
            }
        }
    }
}

void QRWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QColor background(Qt::white);
    painter.setBrush(background);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, width(), height());
    if(qr != NULL)
    {
        draw(painter, width(), height());
    }
}
