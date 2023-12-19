/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_sithread.h"

Test_SiThread::Test_SiThread(QObject *parent) : BaseThread(parent)
{

}

void Test_SiThread::initFunSlot()
{
    mRtu = Dev_SiRtu::bulid(this);
    mCtrl = Dev_SiCtrl::bulid(this);
}

Test_SiThread *Test_SiThread::bulid(QObject *parent)
{
    static Test_SiThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Test_SiThread(parent);
    return sington;
}

bool Test_SiThread::curAlarmWrite(int i)
{
    bool ret = mCtrl->setCurTh(i);
    QString str = tr("电流报警阈值 L%1 写入").arg(i+1);
    if(ret) str += tr("正常"); else str += tr("错误");

    return updatePro(str, ret);
}

bool Test_SiThread::volAlarmWrite(int i)
{
    bool ret = mCtrl->setVolTh(i);
    QString str = tr("电压报警阈值 L%1 写入").arg(i+1);
    if(ret) str += tr("正常"); else str += tr("错误");
    return updatePro(str, ret);
}

bool Test_SiThread::envAlarmWrite()
{
    bool ret = mCtrl->setTem();
    QString str = tr("温度报警阈值写入");
    if(ret) str += tr("正常"); else str += tr("错误");
    ret = updatePro(str, ret);

    ret = mCtrl->setHum();
    str = tr("湿度报警阈值写入");
    if(ret) str += tr("正常"); else str += tr("错误");
    return  updatePro(str, ret);
}

bool Test_SiThread::writeAlarmTh()
{
    bool ret = true;
    int size = mDev->data.size;

    for(int i=0; i<size; ++i) {
        ret = curAlarmWrite(i); if(!ret) break;
        if(i==2 || i==3)sleep(2);
        ret = volAlarmWrite(i); if(!ret) break;
        if(i==2 || i==3)sleep(2);
    }

    return ret;
}

bool Test_SiThread::setAlarm()
{
    mRtu->readPduData();
    bool ret  = writeAlarmTh();
    if(ret) ret = envAlarmWrite();

    return ret;
}

bool Test_SiThread::clearEle()
{
    QString str = tr("清除设备电能");
    bool ret = mCtrl->clearEle();
    if(ret) {
        str += tr("正常");
    } else {
        str += tr("错误");
    }

    return  updatePro(str, ret);
}

bool Test_SiThread::factorySet()
{
    bool ret = true;
    if(mCfg->si_standar) {
        QString str = tr("设备模式切换");
        ret = mCtrl->factorySet();
        if(ret) str += tr("正常"); else str += tr("错误");
        updatePro(str, ret);
    }

    return ret;
}

bool Test_SiThread::setHorizontalOrVertical()
{
    QString str = tr("切换水平");
    bool ret = true;
    if(mDev->dt.screen == 0) {
        if(mCfg->si_horizontal == 0x01) str = tr("切换垂直");
        bool ret = mCtrl->setHorizontaOrVertical();
        if(ret) {
            str += tr("正常");
        } else {
            str += tr("错误");
        }
       ret =  updatePro(str, ret);
    }


    return  ret;
}

bool Test_SiThread::readDev()
{
    bool ret = true;
    for(int i=0; i<5; ++i) {
        QString str = tr("开始读取设备数据：第%1次").arg(i);if(i>1)updatePro(str);
        ret = mRtu->readPduData(); if(ret) break; else if(!delay(5+i)) return false;
        if(i>3)Rtu_Modbus::bulid(this)->get()->changeBaudRate();
    }

    for(int i=0; i<5; ++i) {
        if(mData->size) break; else if(!delay(3)) break;
        ret=mRtu->readPduData();
    }

    QString str = tr("Modbus-RTU通讯 ");
    if(mData->size) {
        if(ret) str += tr("正常"); else str += tr("错误");
    } else {
        str = tr("未识别到相数"); ret = false;
    }

    return  updatePro(str, ret);
}

bool Test_SiThread::checkLine()
{
    bool ret = mRtu->readPduData();
    QString str = tr("设备相数验证");
    if(mCfg->si_lines == 1) {
        int lines = mDev->data.size;
        ret = mDt->lines == lines ? true:false;
    } else if(mCfg->si_lines == 2){
        if(mDt->lines != 2) ret = false;
    }

    if(ret) str += tr("正常"); else str += tr("错误");
    return updatePro(str, ret);
}

bool Test_SiThread::setStandard()
{
    bool ret = true;
    if(mCfg->si_standar) {
        for(int i=0; i<2; ++i) {
            ret = mCtrl->sentRtuCmd(0x1019, 0);
            if(ret) break; else delay(2);
        }
        if(!ret) ret = mCtrl->sentRtuCmd(0x1019, 0, 0x06);
    } else ret = delay(2);

    return true;
}

bool Test_SiThread::initDev()
{
    QString str = tr("等待设备稳定");
    if(mCfg->si_led) {
        mDt->devType = SI_PDU;
        mDt->ac = mCfg->si_ac;
        //mDt->specs = Transformer;
        mDt->lines = mDev->data.size;
        mDt->dev_type = tr("SI/BM数码管");
    }

    updatePro(str, true, 8);
    return setStandard();
}

bool Test_SiThread::setData()
{
    bool ret = mRtu->readPduData();
    //if(!mCfg->si_led)
    //{
        QString str = tr("解锁设备");
        ret = mCtrl->unClock(); if(!ret) return true;
        if(ret) str += tr("成功"); else str += tr("错误");
        ret = updatePro(str, ret);
        if(ret) {
            str = tr("设备配置信息写入");
            ret = mCtrl->setDev();
            if(ret) str += tr("正常"); else str += tr("错误");
            ret = updatePro(str, ret);
        }
    //}

    return  ret;
}

bool Test_SiThread::setDev()
{
    bool ret = initDev();
    if(ret) ret = readDev();
    if(mPro->step <= Test_Seting) {
        if(ret) ret = clearEle();
        if(ret) ret = setData();
        if(ret) ret = checkLine();
        //if(ret) ret = setAlarm();
        if(ret) ret = setHorizontalOrVertical();
        if(ret) ret = factorySet();
    } else {
        if(mPro->step > Test_Collect)
            ret = delay(10);
    }

    return ret;
}
