/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_network.h"

Test_NetWork::Test_NetWork(QObject *parent) : BaseThread(parent)
{

}

Test_NetWork *Test_NetWork::bulid(QObject *parent)
{
    static Test_NetWork* sington = nullptr;
    if(sington == nullptr)
        sington = new Test_NetWork(parent);
    return sington;
}

void Test_NetWork::initFunSlot()
{    
    mProcess = new QProcess(this);
    mUdp = new UdpRecvSocket(this);
    mUdp->initSocket(10086);
    this->start();
}

bool Test_NetWork::checkNet()
{
    bool ret = true;
    QString ip = "192.168.1.163";
    QString str = tr("检测设备网络通讯");
    for(int i=0; i<3; ++i) {
        ret = cm_pingNet(ip);
        if(ret) break; else delay(6);
    }
    if(ret) str += tr("正常"); else str += tr("错误");
    return updatePro(str+ip, ret);
}

bool Test_NetWork::startProcess()
{
    bool ret = checkNet();
    if(ret) updateMacAddr(1); else return ret;

    int t = 33; mac = true;
    QString exe = "pyweb_ctrlset_ip.exe";
    try {
        mProcess->close();
        mProcess->start(exe);
        updatePro(tr("正在启动网页"));
        if(mCfg->security) t = 240;
        if(mPro->step == Test_Seting) t = 240;
        if(ret) mProcess->waitForFinished(t*1000);
//        if(!mac) updateMacAddr(-1);
    } catch (...) {
        return updatePro(tr("Web网页设置异常"), false);
    }

    return ret;
}

bool Test_NetWork::addMacAddr1()
{
    bool ret = checkNet();
    if(ret) updateMacAddr(1); else return ret;
    return ret;
}


void Test_NetWork::updateMacAddr(int step)
{
    BaseLogs::bulid()->writeMac(mItem->mac);
    mPro = sDataPacket::bulid()->getPro();
    MacAddr *mac = MacAddr::bulid();
    // mItem->mac = mac->macAdd(mItem->mac, step);
    QString url = "mac/test?work_order=%1&serial_id=%2";
    url = url.arg(mPro->clientName).arg(mPro->productSN);
    sleep(1);
    QString temp = Json_Pack::bulid()->http_get(url,"192.168.1.12");
    QJsonDocument jsonDoc = QJsonDocument::fromJson(temp.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();
    mItem->mac = jsonObj["mac_address"].toString();
    mPro->macAddress = mItem->mac;
    Cfg::bulid()->write("mac", mItem->mac, "Mac");
}

void Test_NetWork::workDown()
{
    QReadLocker locker(mUdp->getLock());
    UdpBaseData *res = mUdp->getData();
    if(res) {
        QStringList list = QString(res->datagram).split(";");
        if(list.size() == 2) {
            QString str = list.first();
            bool pass = list.last().toInt();
            updatePro(str, pass, 0);
        } else {
            if(QString(res->datagram).contains("MAC-1")) mac = false; else
                qDebug() <<"Test_NetWork workDown err" << list.size();
        }
        delete res;
    } else {
        msleep(1);
    }
}

void Test_NetWork::run()
{
    isRun = true;
    while (isRun) {
        workDown();
    }
}
