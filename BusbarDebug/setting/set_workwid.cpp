#include "set_workwid.h"
#include "ui_set_workwid.h"
#include "macaddr.h"
#include "dblogs.h"

Set_WorkWid::Set_WorkWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Set_WorkWid)
{
    ui->setupUi(this);
    QTimer::singleShot(450,this,SLOT(initFunSlot()));
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
    ui->groupBox_3->hide();

}

Set_WorkWid::~Set_WorkWid()
{
    delete ui;
}

void Set_WorkWid::initFunSlot()
{
    mCnt = 0;
    initTypeComboBox();
    ui->groupBox_2->setEnabled(false);
    ui->groupBox_3->setEnabled(false);
    ui->groupBox_4->setEnabled(false);
    ui->userEdit->setText(mItem->user);
    ui->addrSpin->setValue(mItem->addr);
    ui->cntSpin->setValue(mItem->cnt.cnt);
//    ui->aiBox->setCurrentIndex(0);

    initMac();
    timer = new QTimer(this);
    timer->start(3*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));

    DbLogs *db = DbLogs::bulid();
    connect(db, SIGNAL(itemChanged(int, int)),this, SLOT(updateSlot(int,int)));
}


void Set_WorkWid::saveFunSlot()
{
    bool en = mCnt % 2;
    emit enabledSig(en);
    if(!en) Cfg::bulid()->writeCfgDev();
}

void Set_WorkWid::on_setBtn_clicked()
{
    QString str = tr("修 改");
    bool en = ++mCnt % 2;
    if(en) str = tr("保 存");

    ui->setBtn->setText(str);
    ui->addrSpin->setEnabled(en);
    ui->groupBox_2->setEnabled(en);
    ui->groupBox_3->setEnabled(en);
    ui->groupBox_4->setEnabled(en);
    ui->typeComboBox->setDisabled(en);
    mItem->addr = ui->addrSpin->value();
    mItem->cntMac = ui->spinBox->value();
    mItem->cnt.cnt = ui->cntSpin->value();
    //mItem->aiMode = ui->aiBox->currentIndex();
    if(mItem->user != ui->userEdit->text()) {
        mItem->user = ui->userEdit->text();
        sCount *cnt = &(mItem->cnt);
        cnt->all = cnt->ok = cnt->err = 0;
        Cfg::bulid()->writeCnt();
    }

    QTimer::singleShot(50,this,SLOT(saveFunSlot()));
}

void Set_WorkWid::saveErrSlot()
{
    mCnt = 1;
    emit enabledSig(true);
    ui->setBtn->setText(tr("保 存"));
}


void Set_WorkWid::on_typeComboBox_currentIndexChanged(int index)
{
    mItem->modeId = index;
    initTypeComboBox();
    Cfg::bulid()->writeCfgDev();
    switch (index) {
    case START_BUSBAR:{
        mItem->addr = 1;
        break;
    }
    case INSERT_BUSBAR:{
        mItem->addr = 2;
        break;
    }
    }
    ui->addrSpin->setValue(mItem->addr);
}


void Set_WorkWid::initTypeComboBox()
{
    int index = mItem->modeId;
    emit typeSig(mItem->modeId);
    ui->typeComboBox->setCurrentIndex(index);
}


void Set_WorkWid::updateMac()
{
    int ret =  MacAddr::bulid()->macCnt(mItem->mac, mItem->endMac);
    ui->cntMacLab->setNum(ret);
}


void Set_WorkWid::initMac()
{   
    updateMac();
    ui->spinBox->setValue(mItem->cntMac);
    ui->startMacLab->setText(mItem->startMac);
    ui->endMacLab->setText(mItem->endMac);
}

void Set_WorkWid::updateSlot(int,int)
{
    initMac();
    ui->cntSpin->setValue(mItem->cnt.cnt);
}

void Set_WorkWid::timeoutDone()
{
    updateMac();
}

void Set_WorkWid::on_userEdit_textChanged(const QString &arg1)
{
    ui->userEdit->setClearButtonEnabled(1);
}

