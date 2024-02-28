#ifndef AD_RESULTING_H
#define AD_RESULTING_H

#include "ad_adjusting.h"
#include "dev_ipcfg.h"

class Ad_Resulting : public BaseThread
{
    Q_OBJECT
    explicit Ad_Resulting(QObject *parent = nullptr);
public:
    static Ad_Resulting *bulid(QObject *parent = nullptr);

    bool resEnter();
    bool resEnterOneByOne();
    bool initRtuThread();
    bool eachCurCheckInAdjusting(int k, int exValue);
    bool volErrRangeByIDInAdjusting(int i);
    bool curRangeByIDInAdjusting(int i, int exValue);

    void setStartInfo();
    void setStartValue();
    void setStartLineValue();
    void setStartOtherValue();
    void setStartEnvValue();
    void setStartMACAddress();

    void compareStartInfo();
    void compareStartValue();
    void compareStartLineValue();
    void compareStartOtherValue();
    void compareStartEnvValue();
    void compareStartMac();

    QString changeCurType(int index);
    QString changeMode(int index);
    QString changeBaudType(int index);

    QVector<int> getStartLineValue();
    QVector<int> getStartOtherValue();
    bool check(QString name , QString str , QString r, double rate , int curValue , int expect);

    void setInsertValue();
    void setInsertInfo();
    void setEnvInfo();
    void setInsertLineValue();
    void setInsertEnvValue();
    void setEnvValue();
    void setInsertZeroLineValue();
    bool setStartCurtype();

    void compareInsertValue();
    void compareInsertInfo();
    void compareEnvInfo();
    void compareInsertLineValue();
    void compareInsertEnvValue();
    void compareInsertZeroLineValue();
    void compareEnvValue();

protected:
    bool powErrRange(int exValue, int cur);
    bool curErrRange(int exValue, int cur);

    bool curRangeByID(int i, int exValue, int cnt);
    bool powRangeByID(int i, int exValue, int cnt);
    bool volErrRangeByID(int i);
    bool volErrRange();
    bool workDown(int exValue);
    bool workDownOneByOne(int exValue);

    bool eachCurEnter(int exValue);
    bool eachCurEnterOneByOne(int exValue);
    bool eachCurCheck(int k, int exValue);

    bool noLoadEnter();
    bool noLoadCurFun();
    bool noLoadCurCheck(int cnt);
    bool powerOn();

private:
    YC_StandSource *mSource;
    Dev_Object *mCollect;
    Ctrl_ZpduThread *mControlOp;
    sObjectData *mObj;
};

#endif // AD_RESULTING_H
