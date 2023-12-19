/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "ad_ledsi.h"

Ad_LedSi::Ad_LedSi(QObject *parent) : BaseThread(parent)
{
    mRtu = Dev_SiRtu::bulid();
}

Ad_LedSi *Ad_LedSi::bulid(QObject *parent)
{
    static Ad_LedSi* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_LedSi(parent);
    return sington;
}

bool Ad_LedSi::transmit(uchar *buf, int len)
{
    bool ret = false;
    uchar recv[64] = {0};
    len = mModbus->transmit(buf, len, recv, 20);
    if(len > 0) {
        ret = recvStatus(recv, len);
    } else {
        qDebug() << "Ad_LedSi err " << len;
    }

    return ret;
}

bool Ad_LedSi::writeCmd(uchar fn, uchar line)
{
    uchar cmd[] = {0x7B, 0xA0, 0x00, 0x00, 0x00, 0xBB, 0xBB};
    int len = sizeof(cmd);

    cmd[1] = fn;
    cmd[2] = line;
    uchar crc = mModbus->xorNum(cmd, len-1);
    cmd[len-1] = crc;

    return transmit(cmd, len);
}

bool Ad_LedSi::writeCmd1(uchar fn, uchar line)
{
    uchar cmd[] = {0x7B, 0x00, 0xA0, 0x00, 0x66, 0xBB, 0xBB};
    int len = sizeof(cmd);

    cmd[2] = fn;
    cmd[3] = line;

    ushort crc = mModbus->rtu_crc(cmd, len-2);
    cmd[len-2] = ((0xff) & crc);
    cmd[len-1] = (crc >> 8);

    return transmit(cmd, len);
}



bool Ad_LedSi::writeDc()
{
    bool ret = true;
    bool ans = writeCmd1(0xA0, 0);
    if(!ans){
        delay(5); ans = writeCmd1(0xA0, 0);  // 重复发一次命令
        if(!ans) return ans;
    }
    for(int i=0; i<3; ++i) {
        updatePro(tr("发送直流偏移命令 %1").arg(i+1));
        ret = writeCmd1(0xA2, 0); delay(15);
        if(ret) break;
        //ret = mRtu->readPduData(); if(!ret) ret = mRtu->readPduData();
        //if(mData->cur.value[0]) ret = false; else break; delay(5);
//        ret = waitDcRecv();
//        if(ret) break;
    }

    if(!ret) return ret;
    updatePro(tr("设置标准源电流6A"));
    ret = YC_Dc107::bulid()->setCur(60, 10);
    if(ret) ret = writeCmd1(0xA1, 0);

    return ret;
}

bool Ad_LedSi::writeAc()
{
    bool res = true;
    int line = mDt->lines;
    updatePro(tr("发送校准解锁命令！"));

    bool ans = writeCmd1(0xA0, 0);
    if(!ans){
        delay(5); ans = writeCmd1(0xA0, 0);  // 重复发一次命令
        if(!ans) return ans;
    }
//    for(int i=0; i<line; ++i) {
//        QString str = tr("L%1 校准").arg(i+1);
//        bool ret = writeCmd1(0xA2 , i);
//        if(ret) str += tr("正常"); else {str += tr("错误"); res = false;}
//        updatePro(str, true, 7);
//    }
    updatePro(tr("发送启动校准命令！"),res, 1);
    res = writeCmd1(0xA2, 0);

    return res;
}


bool Ad_LedSi::sentCmd()
{
    bool ret = true;

    if(DC == mDt->ac) {
        writeDc();
    } else {
        writeAc();
    }

    return ret;
}

bool Ad_LedSi::startAdjust()
{
    bool ret = sentCmd();
    if(ret) {
//        if(mItem->aiMode == Test_AI) mPro->step = Test_Over;
//        else mPro->step = Test_vert;
    } else {
        mPro->step = Test_vert;
    }

    return updatePro(tr("校准返回！"), ret, 1);
}

bool Ad_LedSi::waitDcRecv()
{
    bool ret = false;
    uchar buf[MODBUS_RTU_SIZE] = {0};
    updatePro(tr("正在等待直流偏移：请等待..."));

    int len = readSerial(buf, 30);
    if(len > 0){
        ret = recvStatus(buf, len);
    } else {
        ret = overWork(tr("直流偏移等待超时！"));
    }

    return ret;
}

bool Ad_LedSi::updateStatus(ushort status)
{
    QString str;
    if(0x1100 == status) {
        uchar step = Test_vert;
        str = tr("校准返回正常！");
//        if(mItem->aiMode == Test_AI) step = Test_Over;
        mPro->step = step;
    } else if(0x1101 == status) {
        str = tr("校准失败");
        mPro->step = Test_vert;
    } else if(0x1102 == status) {
        str = tr("校准解锁成功");
    } else if(0x1108 == status) {
        str = tr("直流偏移校准成功");
    }else if(0x1109 == status) {
        str = tr("直流偏移校准失败");
    }else if(0x110A == status) {
        str = tr("直流正在校准");
    }else if(0x110B == status) {
        str = tr("直流电流校准失败");
    }else if(0x110C == status) {
        str = tr("直流电压校准失败");
    }else if(status <= 0x1115) {
        if(status%3 == 0) {
            str = tr("L%1相， 正在校准").arg((status-0x110D)/3+1);
        } else if(status%3 == 1) {
            str = tr("L%1相， 校准成功").arg((status-0x110D)/3+1);
        } else if(status%3 == 2) {
            str = tr("L%1相， 校准失败").arg((status-0x110D)/3+1);
        }
    } else if(status <= 0x1118) {
        str = tr("校准失败：L%1相电流 ").arg(status-0x1115);
    } else if(status <= 0x111C) {
        str = tr("校准失败：L%1相电压 ").arg(status-0x1119);
    }else if(status <= 0x112F) {
        str = tr("校准完成，输出位%1 ").arg(status-0x1120);
    } else if(status <= 0x114F) {
        str = tr("电流校准失败：输出位%1").arg(status-0x1140);
    } else if(status <= 0x116F) {
        str = tr("电压校准失败：输出位%1").arg(status-0x1160);
    } else if(status <= 0x1172) {
        str = tr("第%1相偏移（0A）校准完成状态").arg(status-0x1170+1);
    } else {
        str = tr("校准失败：状态返回错误%1 ").arg(QString::number(status, 16));
    }

    return updatePro(str);
}

bool Ad_LedSi::overWork(const QString &str)
{
    mPro->step = Test_End;
    return updatePro(str, false, 0);
}

bool Ad_LedSi::recvStatus(uchar *recv, int len)
{
    bool ret = true;
    if((len>0) && (len%8 == 0)) {
        for(int i = 0 ; i < len ; i+=8) {
            ushort status = recv[i+4]*256 + recv[i+5];
            ret = updateStatus(status);
        }
    } else {
        //ret = false;
        qDebug() << "Adjust res len err" << len;
    }

    return ret;
}

int Ad_LedSi::readSerial(uchar *recv, int sec)
{
    uchar *ptr = nullptr;
    uchar buf[MODBUS_RTU_SIZE] = {0};

    int len = mModbus->readSerial(buf, sec);
    if(len > 0){
        if(len > 8) {
            ptr = &(buf[len-8]);
            len = 8;
        } else {
            ptr = buf;
        }

        for(int i=0; i<len; ++i) {
            recv[i] = ptr[i];
        }
    }

    return len;
}
