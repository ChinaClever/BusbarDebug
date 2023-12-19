#ifndef PRINTERWID_H
#define PRINTERWID_H
#include <QtCore>
#include <QWidget>
#include <QPrinter>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrintDialog>
#include <QDebug>
#include <QPixmap>

namespace Ui {
class PrinterWid;
}

class PrinterWid : public QWidget
{
    Q_OBJECT

public:
    explicit PrinterWid(QWidget *parent = nullptr);
    ~PrinterWid();

    void printerWid(QWidget *wid);


private slots:
    void printPreviewSlot(QPrinter*);
    void on_printsBtn_clicked();
    void on_previewBt_clicked();
    void on_saveBtn_clicked();

private:
    Ui::PrinterWid *ui;
};

#endif // PRINTERWID_H
