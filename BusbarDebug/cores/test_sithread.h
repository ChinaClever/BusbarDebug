#ifndef TEST_SICTRL_H
#define TEST_SICTRL_H

#include "test_network.h"
#include "dev_ipcfg.h"

class Test_SiThread : public BaseThread
{
    Q_OBJECT
    explicit Test_SiThread(QObject *parent = nullptr);
public:
    static Test_SiThread *bulid(QObject *parent = nullptr);

    bool setDev();
    bool readDev();
    bool clearEle();
    bool setAlarm();

protected:
    bool initDev();
    bool setData();
    bool checkLine();
    bool setStandard();
    bool volAlarmWrite(int i);
    bool curAlarmWrite(int i);
    bool envAlarmWrite();
    bool writeAlarmTh();

    bool factorySet();
    bool setHorizontalOrVertical();

protected slots:
    void initFunSlot();

private:
    Dev_SiRtu *mRtu;
    Dev_SiCtrl *mCtrl;
};

#endif // TEST_SICTRL_H
