#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "yc_obj.h"
#include "test_sithread.h"
#include "ad_corethread.h"
#include "ctrl_zpduthread.h"

class Test_CoreThread : public BaseThread
{
    Q_OBJECT
public:
    explicit Test_CoreThread(QObject *parent = nullptr);

protected:
    void run();
    bool initFun();
    bool setDev();
    bool readDev();
    bool checkDev();
    bool workDown();
    void collectData();
    void workResult();
    void allTest();


protected slots:
    void initFunSlot();

private:
    Yc_Obj *mYc;
    Sn_SerialNum *mSn;
    Test_SiThread *mCtrl;
    Ad_CoreThread *mAd;
    Ctrl_ZpduThread *mControlOp;
};

#endif // TEST_CORETHREAD_H
