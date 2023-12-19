#ifndef QRWIDGET_H
#define QRWIDGET_H
#include <QWidget>
#include "qrhelper.h"

class QRWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QRWidget(QWidget *parent = nullptr);
    ~QRWidget();

    void setString(QString str, int v=3);
    int getQRWidth() const;
    bool saveImage(QString name);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

private:
    void draw(QPainter &painter, int width, int height);
    QString string;
    QRcode *qr;
};

#endif // QRWIDGET_H
