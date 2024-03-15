#include "set_temper.h"
#include "ui_set_temper.h"

Set_temper::Set_temper(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Set_temper)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    QTimer::singleShot(13,this,SLOT(initFunSlot()));
}

Set_temper::~Set_temper()
{
    delete ui;
}
void Set_temper::initFunSlot()
{
    this->setEnabled(false);
    mObj = Dev_SiCfg::bulid(this);
    mItem = Cfg::bulid()->item;
    mDev = mObj->getDev();
    initType();
}
void Set_temper::initType()
{
    sTypeCfg *dt = &(mDev->cfg); //设备类型
    ui->tem_minSpinBox_1->setValue(dt->temMin[0]);
    ui->tem_minSpinBox_2->setValue(dt->temMin[1]);
    ui->tem_minSpinBox_3->setValue(dt->temMin[2]);
    ui->tem_minSpinBox_4->setValue(dt->temMin[3]);
    ui->tem_maxSpinBox_1->setValue(dt->temMax[0]);
    ui->tem_maxSpinBox_2->setValue(dt->temMax[1]);
    ui->tem_maxSpinBox_3->setValue(dt->temMax[2]);
    ui->tem_maxSpinBox_4->setValue(dt->temMax[3]);

    ui->buzzer_comboBox->setCurrentIndex(dt->tem_buzzer);
    ui->filter_comboBox->setCurrentIndex(dt->tem_filter - 2);

}
void Set_temper::updateType()
{
    sTypeCfg *dt = &(mDev->cfg); //设备类型
    dt->temMin[0] = ui->tem_minSpinBox_1->value();
    dt->temMin[1] = ui->tem_minSpinBox_2->value();
    dt->temMin[2] = ui->tem_minSpinBox_3->value();
    dt->temMin[3] = ui->tem_minSpinBox_4->value();
    dt->temMax[0] = ui->tem_maxSpinBox_1->value();
    dt->temMax[1] = ui->tem_maxSpinBox_2->value();
    dt->temMax[2] = ui->tem_maxSpinBox_3->value();
    dt->temMax[3] = ui->tem_maxSpinBox_4->value();
    dt->tem_buzzer = ui->buzzer_comboBox->currentIndex();
    dt->tem_filter = ui->filter_comboBox->currentIndex();
    dt->tem_filter = ui->filter_comboBox->currentIndex() + 2;
}
bool Set_temper::inputCheck()
{
    return true;
}
bool Set_temper::dataSave()
{
    bool ret = inputCheck();
    if(ret) {
        updateType();
    }

    return ret;
}

void Set_temper::enabledSlot(bool en)
{
    if(mItem->modeId != TEMPER_BUSBAR) return;
    this->setEnabled(en);
    if(!en) {
        en = dataSave();
        if(en) {
            mObj->save();
        } else {
            emit errSig();
        }
    }
}
