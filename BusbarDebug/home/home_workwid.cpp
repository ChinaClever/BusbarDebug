/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "home_workwid.h"
#include "ui_home_workwid.h"

Home_WorkWid::Home_WorkWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home_WorkWid)
{
    ui->setupUi(this);
    mPro = sDataPacket::bulid()->getPro();
    QTimer::singleShot(450,this,SLOT(initFunSlot()));
}

Home_WorkWid::~Home_WorkWid()
{
    delete ui;
}

void Home_WorkWid::initLayout()
{
    QPalette pl = ui->textEdit->palette();
    pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    ui->textEdit->setPalette(pl);
    //ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGridLayout *gridLayout = new QGridLayout(this->parentWidget());
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

void Home_WorkWid::initFunSlot()
{
    int cnt = MacAddr::bulid()->macCnt(mItem->startMac, mItem->mac);
    ui->macCntLab->setNum(cnt);
    ui->cntLab->setNum(mItem->cnt.cnt);
    ui->userLab->setText(mItem->user);
    mPro->step = Test_End;

    initLayout();
    timer = new QTimer(this);
    timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    connect(Json_Pack::bulid(this), &Json_Pack::httpSig, this, &Home_WorkWid::insertTextSlot);
    mCoreThread = new Test_CoreThread(this);
    ui->label_9->hide();
    ui->typeComboBox->hide();
}

void Home_WorkWid::setTextColor()
{
    QColor color("black");
    bool pass = mPro->pass.first();
    if(!pass) color = QColor("red");
    ui->textEdit->moveCursor(QTextCursor::Start);

    QTextCharFormat fmt;//文本字符格式
    fmt.setForeground(color);// 前景色(即字体色)设为color色
    QTextCursor cursor = ui->textEdit->textCursor();//获取文本光标
    cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
    ui->textEdit->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式
}

void Home_WorkWid::insertText()
{
    while(mPro->status.size()) {
        setTextColor();
        QString str = QString::number(mId++) + "、"+ mPro->status.first() + "\n";
        ui->textEdit->insertPlainText(str);
        mPro->status.removeFirst();
        mPro->pass.removeFirst();
    }
}

void Home_WorkWid::insertTextSlot(QString str , bool ret)
{
    mPro->itemName<<str;
    mPro->uploadPass<<ret;
}

void Home_WorkWid::updateCnt()
{
    sCount *cnt = &(mItem->cnt);
    ui->okLcd->display(cnt->ok);
    ui->allLcd->display(cnt->all);
    ui->errLcd->display(cnt->err);
    ui->macLab->setText(mItem->mac);

    QString str = "0";
    if(cnt->all) {
        double value = cnt->ok*100.0 / cnt->all;
        str = QString::number(value,'f',0) +"%";
    }
    ui->passLcd->display(str);
    ui->typeComboBox->setCurrentIndex(mItem->modeId);
}

QString Home_WorkWid::getTime()
{
    QTime t(0,0,0,0);
    t = t.addSecs(mPro->startTime.secsTo(QTime::currentTime()));
    return  tr("%1").arg(t.toString("mm:ss"));
}

void Home_WorkWid::updateTime()
{
    QString str = getTime();
    QString style = "background-color:yellow; color:rgb(0, 0, 0);";
    style += "font:100 34pt \"微软雅黑\";";

    ui->timeLab->setText(str);
    ui->timeLab->setStyleSheet(style);
    ui->startBtn->setText(tr("终 止"));
}

void Home_WorkWid::updateResult()
{
    QString style;
    QString str = tr("---");
    //    if(mItem->modeId && isCheck) {
    //        QString str = getTime().at(1);
    //        if(!str.toInt() || (mId<40)) mPro->result = Test_Fail;
    //    } else if(isCheck) {
    //        if(mId < 14) mPro->result = Test_Fail;
    //    }
    if(mId < 30) mPro->result = Test_Fail;

    if (Test_Fail == mPro->result) {
        str = tr("失败");
        style = "background-color:red; color:rgb(255, 255, 255);";
    } else {
        str = tr("成功");
        style = "background-color:green; color:rgb(255, 255, 255);";
    }
    style += "font:100 34pt \"微软雅黑\";";

    mPro->step = Test_End;
    ui->timeLab->setText(str);
    ui->modeBox->setEnabled(true);
    ui->groupBox_4->setEnabled(true);
    ui->timeLab->setStyleSheet(style);
    ui->startBtn->setText(tr("开 始"));
    ui->cntLab->setNum(mItem->cnt.cnt);
    int cnt = MacAddr::bulid()->macCnt(mItem->startMac, mItem->mac);
    ui->macCntLab->setNum(cnt);
    if(mItem->cnt.cnt < 1) {
        mItem->user.clear();
        ui->userLab->setText(mItem->user);
    }
}

void Home_WorkWid::updateWid()
{
    QString str = mDt->sn;
    sObjData *obj = &(mDev->data);
    if(str.isEmpty()) str = "--- ---";
    ui->snLab->setText(str);
    mPro->productSN = str;

    // str = mItem->modeId == START_BUSBAR?tr("始端箱"):tr("插接箱");
    str = mDt->dev_type;
    mPro->productType = str;
    ui->devLab->setText(str);

    if(str.isEmpty()) str = "--- ---";
    int ver = mDataBusbar->box[mItem->addr-1].version;
    if(ver != 0)str += "  版本："+QString::number(ver/100)+"."+QString::number(ver/10%10)+"."+QString::number(ver%10);
    mPro->softwareVersion = QString::number(ver/100)+"."+QString::number(ver/10%10)+"."+QString::number(ver%10);

    ui->userLab->setText(mItem->user);
    mPro->clientName = mItem->user;


    ui->cntLab->setNum(mItem->cnt.cnt);

    if(mPro->step < Test_Over) {
        updateTime();
    } else if(mPro->step < Test_End){
        updateResult();
    }
}

void Home_WorkWid::timeoutDone()
{
    insertText();
    updateWid();
    updateCnt();
}

bool Home_WorkWid::checkPduPort()
{
    bool ret =  mItem->com->isOpened();
    if(!ret) {MsgBox::critical(this, tr("请先打开PDU串口")); return ret;}
    return ret;
}

bool Home_WorkWid::checkCtrlSourcePort()
{
    bool ret =  mItem->source->isOpened();
    if(!ret) {MsgBox::critical(this, tr("请先打开标准源串口")); return ret;}
    return ret;
}

bool Home_WorkWid::initSerial()
{
    bool ret = true , ret1 = true, ret2 = true;

    if(mItem->modeId == INSERT_BUSBAR) ret1 = checkPduPort();
    if(ui->modeBox->currentIndex() == 0){
        ret2 = checkCtrlSourcePort();
    }
    mPro->PCB_Code = ui->pcbEdit->text();
    ret = ret1 & ret2;
    return ret;
}

bool Home_WorkWid::initWid()
{

    bool ret = initSerial();
    if(ret) {
        if(mItem->user.isEmpty()) {
            MsgBox::critical(this, tr("请先填写工单号！")); return false;
        }
        if(mItem->cnt.cnt < 1) {
            MsgBox::critical(this, tr("请先填写订单剩余数量！")); return false;
        }
        if(mItem->modeId == START_BUSBAR) {
            uint res =  MacAddr::bulid()->macCnt(mItem->mac, mItem->endMac);
            if((res <= mItem->cntMac) && mItem->modeId) {
                if(res < 1) {
                    MsgBox::critical(this, tr("MAC地址已用完，无法继续使用")); return false;
                } else {
                    QString str = tr("剩余MAC地址，仅有%1个，请向领导反馈").arg(res);
                    MsgBox::critical(this, str);
                }
            }
        }

        mId = 1;
        mPacket->init();
        mPro->testStartTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        ui->textEdit->clear();
        ui->modeBox->setEnabled(false);
        ui->groupBox_4->setEnabled(false);
        mPro->step = (ui->modeBox->currentIndex()==0)?Test_Start:Test_Collect;
        if(mPro->step == Test_Start) isCheck = true; else isCheck = false;
    }

    return ret;
}

void Home_WorkWid::on_startBtn_clicked()
{
    if(mPro->step == Test_End) {
        if(!ui->pcbEdit->text().isEmpty()){
            if(initWid()) mCoreThread->start();
        }else{
            MsgBox::critical(this, tr("请先填写pcb码！"));
        }
    } else {
        bool ret = MsgBox::question(this, tr("确定需要提前结束？"));
        if(ret) {
            mPro->result = Test_Fail;
            updateResult();
        }
    }
}

void Home_WorkWid::on_upBtn_clicked()
{
    Yc_Obj::bulid()->get()->setVol(220);
}

void Home_WorkWid::on_downBtn_clicked()
{
    Yc_Obj::bulid()->get()->setCur(0);
    Yc_Obj::bulid()->get()->setVol(0);
}

void Home_WorkWid::on_pcbEdit_textChanged(const QString &arg1)
{
    ui->pcbEdit->setClearButtonEnabled(1);
}

