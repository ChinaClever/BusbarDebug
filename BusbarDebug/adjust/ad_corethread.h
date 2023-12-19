#ifndef AD_CORETHREAD_H
#define AD_CORETHREAD_H
#include "ctrl_zpduthread.h"
#include "ad_resulting.h"

class Ad_CoreThread : public BaseThread
{
    Q_OBJECT
    explicit Ad_CoreThread(QObject *parent = nullptr);
public:
    static Ad_CoreThread *bulid(QObject *parent = nullptr);
    bool verifyResult() {
        bool ret = false;
        if(mPro->step == Test_vert) {
//            if(mItem->aiMode == Test_Onebyone)
//                ret = mResult->resEnterOneByOne();
//            else
//                ret = mResult->resEnter();
        }
        return ret;
    }
    bool startAdjust() {return workDown();}

protected:
    bool workDown();

private:
    Ad_Adjusting *mAdjust;
    Ad_Resulting *mResult;
    Ad_LedSi *mLedSi;
    Ctrl_ZpduThread *mControlOp;
};

#endif // AD_CORETHREAD_H
