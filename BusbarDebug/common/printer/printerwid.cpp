#include "printerwid.h"
#include "ui_printerwid.h"

PrinterWid::PrinterWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrinterWid)
{
    ui->setupUi(this);
    //ui->lab_img->setGeometry(400,50,200,300);
    //ui->lab_img->setStyleSheet("background-image:url(:/img/img/dog.png)");
}

PrinterWid::~PrinterWid()
{
    delete ui;
}

void PrinterWid::printerWid(QWidget *wid)
{
    QPixmap pixmap = QPixmap::grabWidget(wid, wid->rect());
    ui->lab_img->setPixmap(pixmap);
    ui->lab_img->adjustSize();
}

void PrinterWid::on_printsBtn_clicked()
{
   QPrinter printerPixmap;
   QPixmap pixmap = QPixmap::grabWidget(ui->lab_img, ui->lab_img->rect());  //获取界面的图片
   printerPixmap.setOrientation(QPrinter::Portrait);//纵向：Portrait 横向：Landscape
   printerPixmap.setPageSize(QPrinter::A4);//设置纸张大小
   QPainter painterPixmap;
   painterPixmap.begin(&printerPixmap);
   painterPixmap.scale(1, 1);//设置图像长宽是原图的多少倍
   painterPixmap.drawPixmap(300, 300 , pixmap);//设置图像在A4中的开始坐标是什么
   painterPixmap.end();
}

void PrinterWid::printPreviewSlot(QPrinter* printerPixmap)
{
    printerPixmap->setOrientation(QPrinter::Landscape);
    QPixmap pixmap = QPixmap::grabWidget(ui->lab_img, ui->lab_img->rect()); //获取界面的图片
    QPainter painterPixmap(this);
    painterPixmap.begin(printerPixmap);
    QRect rect = painterPixmap.viewport();
    int x = rect.width() / pixmap.width();
    int y = rect.height() / pixmap.height();
    painterPixmap.scale(x, y);
    painterPixmap.drawPixmap(0, 0, pixmap);
    painterPixmap.end();
}

void PrinterWid::on_previewBt_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    preview.setMinimumSize(1000,600);
    printer.setPageSize(QPrinter::Custom); //自定义纸张大小
    printer.setPaperSize(QSizeF(ui->lab_img->height(), ui->lab_img->width()), QPrinter::Point);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreviewSlot(QPrinter*)));
    preview.exec();
}

void PrinterWid::on_saveBtn_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存PDF文件"), QString(), "*.pdf");
    if (!fileName.isEmpty())
    {
        // 如果文件后缀为空，则默认使用.pdf
        if (QFileInfo(fileName).suffix().isEmpty())  fileName.append(".pdf");
        QPrinter printerPixmap(QPrinter::HighResolution);
        //自定义纸张大小，这里要打印的内容都在stackedlab_img上
        printerPixmap.setPageSize(QPrinter::Custom);
        printerPixmap.setPaperSize(QSizeF(ui->lab_img->height(), ui->lab_img->width()), QPrinter::Point);
        //设置纸张大小为A4，这里注释掉了，建议自定义纸张 ，否则保存的就会有很多空白
        //printerPixmap.setPageSize(QPrinter::A4);

        printerPixmap.setOrientation(QPrinter::Landscape);  //横向打印
        printerPixmap.setOutputFormat(QPrinter::PdfFormat); //设置输出格式为pdf
        printerPixmap.setOutputFileName(fileName); //设置输出路径
        QPixmap pixmap = QPixmap::grabWidget(ui->lab_img, ui->lab_img->rect());  //获取界面的图片
        QPainter painterPixmap;
        painterPixmap.begin(&printerPixmap);
        QRect rect = painterPixmap.viewport();
        int x = rect.width() / pixmap.width();
        int y = rect.height() / pixmap.height();

        //将图像(所有要画的东西)在pdf上按比例尺缩放,如果想展示原始图，就讲x和y设置为1就可以了
        painterPixmap.scale(x, y);
        painterPixmap.drawPixmap(0, 0, pixmap); //画图
        painterPixmap.end();

        QMessageBox::information(this, tr("生成PDF"), tr("保存PDF文件成功"), QMessageBox::Ok);
    }
}
