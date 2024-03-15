#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>
#include <QColor>

#define LINE_NUM  3
#define PACK_ARRAY_SIZE LINE_NUM
#define OpSize  6

// 倍率定义
#define COM_RATE_VOL	10.0    // 电压
#define COM_RATE_CUR	1000.0    // 电流
#define COM_RATE_POW	1000.0  // 功率
#define COM_RATE_ELE	10.0    // 电能
#define COM_RATE_PF     100.0   // 功率因数
#define COM_RATE_TEM	1.0    // 温度
#define COM_RATE_HUM	1.0    // 湿度

#define START_LINE_NUM 3 // 3相
#define LINE_NUM_MAX 9 // 三相
#define LOOP_NUM_MAX 9 // 9回路
#define BOX_NUM 20 // 插接箱数量
#define BUS_NUM 4  // 四条母线
#define NAME_LEN 32 // 名称最大长度
#define SENSOR_NUM 4  // 四个传感器
#define HARMONIC_NUM 32  // 谐波数量

#define RTU_LOOP_NUM 9 // 9回路
#define RTU_LINE_NUM 3 // 3相
#define RTU_TH_NUM 4 // 4个传感器
#define RTU_THD_NUM 4 // 4个谐波含量
//1.4版本
//#define RTU_SENT_LEN (22*RTU_LINE_NUM+1+3*3+1+1+3+11+6)
//1.3版本
//#define RTU_SENT_LEN (22*RTU_LINE_NUM+1+3*3+1+1+3+11+6)  // 长度需要改变 ////============ 加上开关，功率因素之后，是为14 [追加交直流区分]
#define RTU_SENT_LEN 0xFF
#define RTU_SENT_LEN_V23 0x135//V2.3 and V2.4
#define RTU_SENT_LEN_V25 0x280//V2.5
#define RTU_SENT_LEN_V30 0x15E//V3.0.0
#define SRC_DATA_LEN_MAX 1024 //一包数据最长 —— 用于对接动环

enum  sSetStartType{
    StartCurSpecification    = 3
    ,StartWorkMode           = 4
    ,StartBaudRate           = 5           //01:9600 02:9600 03:38400 04:56000
    ,SetStartBuzzer          = 6           //0:开启 1:关闭
    ,SetStartiOF             = 7           //0:没有 1:有
    ,SetStartAlarmTime       = 8           //2-5
    ,SetStartBreaker         = 9           //0:没有 1:正常 2:损坏
    ,SetStartLps             = 10           //0:没有 1:正常 2:损坏

    ,StartEle_1   = 34           //电能清零
    ,StartEle_2   = 51
    ,StartEle_3   = 68

    ,StartTemperatureMIN_1   = 284           //温度上限
    ,StartTemperatureMAX_1   = 285           //温度下限
    ,StartTemperatureMIN_2   = 286
    ,StartTemperatureMAX_2   = 287
    ,StartTemperatureMIN_3   = 288
    ,StartTemperatureMAX_3   = 289
    ,StartTemperatureMIN_4   = 290
    ,StartTemperatureMAX_4   = 291

    ,StartReCurAlarm         = 292
    ,StartZoneCurMIN         = 293
    ,StartZoneCurMAX         = 294

    ,StartTotalPowMIN_1      = 295
    ,StartTotalPowMIN_2      = 296
    ,StartTotalPowMAX_1      = 297
    ,StartTotalPowMAX_2      = 298

    ,StartSetHzMIN           = 299
    ,StartSetHzMAX           = 300

    ,StartLineVoltageMIN_L1  = 301           //线电压下限
    ,StartLineVoltageMAX_L1  = 302           //线电压上限
    ,StartVoltageMIN_L1      = 303           //电压下限
    ,StartVoltageMAX_L1      = 304           //电压上限
    ,StartCurrentMIN_L1      = 305           //电流下限
    ,StartCurrentMAX_L1      = 306           //电流上限
    ,StartPowerMIN_L1_1      = 307           //功率下限
    ,StartPowerMIN_L1_2      = 308           //功率下限
    ,StartPowerMAX_L1_1      = 309           //功率上限
    ,StartPowerMAX_L1_2      = 310           //功率上限

    ,StartLineVoltageMIN_L2  = 311           //电压下限
    ,StartLineVoltageMAX_L2  = 312           //电压上限
    ,StartVoltageMIN_L2      = 313           //电压下限
    ,StartVoltageMAX_L2      = 314           //电压上限
    ,StartCurrentMIN_L2      = 315           //电流下限
    ,StartCurrentMAX_L2      = 316           //电流上限
    ,StartPowerMIN_L2_1      = 317           //功率下限
    ,StartPowerMIN_L2_2      = 318           //功率下限
    ,StartPowerMAX_L2_1      = 319           //功率上限
    ,StartPowerMAX_L2_2      = 320           //功率上限

    ,StartLineVoltageMIN_L3  = 321           //电压下限
    ,StartLineVoltageMAX_L3  = 322           //电压上限
    ,StartVoltageMIN_L3      = 323           //电压下限
    ,StartVoltageMAX_L3      = 324           //电压上限
    ,StartCurrentMIN_L3      = 325           //电流下限
    ,StartCurrentMAX_L3      = 326           //电流上限
    ,StartPowerMIN_L3_1      = 327           //功率下限
    ,StartPowerMIN_L3_2      = 328           //功率下限
    ,StartPowerMAX_L3_1      = 329           //功率上限
    ,StartPowerMAX_L3_2      = 330           //功率上限

};

enum  sSetPlugType{
    SetPlugAddress           = 3           //地址
    ,PlugBaudRate            = 4           //01:9600 02:9600 03:38400 04:56000
    ,SetPlugiOF              = 5           //0:没有 1:有
    ,SetPlugBuzzer           = 6           //0:开启 1:关闭
    ,SetPlugAlarmTime        = 7            //2-5

    ,SetPlugEle_1   = 30           //电能清零
    ,SetPlugEle_2   = 45
    ,SetPlugEle_3   = 60
    ,SetPlugEle_4   = 75           //电能清零
    ,SetPlugEle_5   = 90
    ,SetPlugEle_6   = 105
    ,SetPlugEle_7   = 120           //电能清零
    ,SetPlugEle_8   = 135
    ,SetPlugEle_9   = 150

    ,PlugTemperatureMIN_1    = 165           //温度上限
    ,PlugTemperatureMAX_1    = 166           //温度下限
    ,PlugTemperatureMIN_2    = 167
    ,PlugTemperatureMAX_2    = 168
    ,PlugTemperatureMIN_3    = 169
    ,PlugTemperatureMAX_3    = 170
    ,PlugTemperatureMIN_4    = 171
    ,PlugTemperatureMAX_4    = 172

    ,PlugVoltageMIN_L1       = 173           //电压下限
    ,PlugVoltageMAX_L1       = 174           //电压上限
    ,PlugCurrentMIN_L1       = 175           //电流下限
    ,PlugCurrentMAX_L1       = 176           //电流上限
    ,PlugPowerMIN_L1_1       = 177           //功率下限
    ,PlugPowerMIN_L1_2       = 178           //功率下限
    ,PlugPowerMAX_L1_1       = 179           //功率上限
    ,PlugPowerMAX_L1_2       = 180           //功率上限

    ,PlugVoltageMIN_L2       = 181           //电压下限
    ,PlugVoltageMAX_L2       = 182           //电压上限
    ,PlugCurrentMIN_L2       = 183           //电流下限
    ,PlugCurrentMAX_L2       = 184           //电流上限
    ,PlugPowerMIN_L2_1       = 185           //功率下限
    ,PlugPowerMIN_L2_2       = 186           //功率下限
    ,PlugPowerMAX_L2_1       = 187           //功率上限
    ,PlugPowerMAX_L2_2       = 188           //功率上限

    ,PlugVoltageMIN_L3       = 189           //电压下限
    ,PlugVoltageMAX_L3       = 190           //电压上限
    ,PlugCurrentMIN_L3       = 191           //电流下限
    ,PlugCurrentMAX_L3       = 192           //电流上限
    ,PlugPowerMIN_L3_1       = 193           //功率下限
    ,PlugPowerMIN_L3_2       = 194           //功率下限
    ,PlugPowerMAX_L3_1       = 195           //功率上限
    ,PlugPowerMAX_L3_2       = 196           //功率上限

    ,PlugVoltageMIN_L4       = 197           //电压下限
    ,PlugVoltageMAX_L4       = 198           //电压上限
    ,PlugCurrentMIN_L4       = 199           //电流下限
    ,PlugCurrentMAX_L4       = 200           //电流上限
    ,PlugPowerMIN_L4_1       = 201           //功率下限
    ,PlugPowerMIN_L4_2       = 202           //功率下限
    ,PlugPowerMAX_L4_1       = 203           //功率上限
    ,PlugPowerMAX_L4_2       = 204           //功率上限

    ,PlugVoltageMIN_L5       = 205           //电压下限
    ,PlugVoltageMAX_L5       = 206           //电压上限
    ,PlugCurrentMIN_L5       = 207           //电流下限
    ,PlugCurrentMAX_L5       = 208           //电流上限
    ,PlugPowerMIN_L5_1       = 209           //功率下限
    ,PlugPowerMIN_L5_2       = 210           //功率下限
    ,PlugPowerMAX_L5_1       = 211           //功率上限
    ,PlugPowerMAX_L5_2       = 212           //功率上限

    ,PlugVoltageMIN_L6       = 213           //电压下限
    ,PlugVoltageMAX_L6       = 214           //电压上限
    ,PlugCurrentMIN_L6       = 215           //电流下限
    ,PlugCurrentMAX_L6       = 216           //电流上限
    ,PlugPowerMIN_L6_1       = 217           //功率下限
    ,PlugPowerMIN_L6_2       = 218           //功率下限
    ,PlugPowerMAX_L6_1       = 219           //功率上限
    ,PlugPowerMAX_L6_2       = 220           //功率上限

    ,PlugVoltageMIN_L7       = 221           //电压下限
    ,PlugVoltageMAX_L7       = 222           //电压上限
    ,PlugCurrentMIN_L7       = 223           //电流下限
    ,PlugCurrentMAX_L7       = 224           //电流上限
    ,PlugPowerMIN_L7_1       = 225           //功率下限
    ,PlugPowerMIN_L7_2       = 226           //功率下限
    ,PlugPowerMAX_L7_1       = 227           //功率上限
    ,PlugPowerMAX_L7_2       = 228           //功率上限

    ,PlugVoltageMIN_L8       = 229           //电压下限
    ,PlugVoltageMAX_L8       = 230           //电压上限
    ,PlugCurrentMIN_L8       = 231           //电流下限
    ,PlugCurrentMAX_L8       = 232           //电流上限
    ,PlugPowerMIN_L8_1       = 233           //功率下限
    ,PlugPowerMIN_L8_2       = 234           //功率下限
    ,PlugPowerMAX_L8_1       = 235           //功率上限
    ,PlugPowerMAX_L8_2       = 236           //功率上限

    ,PlugVoltageMIN_L9       = 237           //电压下限
    ,PlugVoltageMAX_L9       = 238           //电压上限
    ,PlugCurrentMIN_L9       = 239           //电流下限
    ,PlugCurrentMAX_L9       = 240           //电流上限
    ,PlugPowerMIN_L9_1       = 241           //功率下限
    ,PlugPowerMIN_L9_2       = 242           //功率下限
    ,PlugPowerMAX_L9_1       = 243           //功率上限
    ,PlugPowerMAX_L9_2       = 244           //功率上限

    ,PlugZeroLineMIN       = 247           //零线电流下限
    ,PlugZeroLineMAX       = 248           //零线电流上限

};

/**
 * 数据单元：包括当前值，阈值，临界值，告警状态等
 */
struct sDataUnit
{
    sDataUnit() {size=0;}

    ushort size;
    ushort value[PACK_ARRAY_SIZE]; // 值
    ushort min[PACK_ARRAY_SIZE]; // 最小值
    ushort max[PACK_ARRAY_SIZE]; // 最大值
    uchar alarm[PACK_ARRAY_SIZE]; // 报警值 0表示未报警  1表示已报警 2表示已记录
    uchar status[PACK_ARRAY_SIZE];
};



/**
 * 数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
struct sObjData
{
    sObjData() {size=0;}
    int size;
    sDataUnit vol; // 电压
    sDataUnit cur; // 电流
    sDataUnit tem; // 温度
    sDataUnit hum; // 湿度

    ushort pow[PACK_ARRAY_SIZE]; // 功率
    ushort powed[PACK_ARRAY_SIZE]; // 功率
    uint ele[PACK_ARRAY_SIZE]; // 电能

    uchar pf[PACK_ARRAY_SIZE]; // 功率因数
    uchar sw[PACK_ARRAY_SIZE]; // 开关状态 0 表示未启用
    ushort aPow[PACK_ARRAY_SIZE]; // 视在功率值
    uchar status[PACK_ARRAY_SIZE];
    ushort cured[PACK_ARRAY_SIZE]; // 电流

    ushort hz[PACK_ARRAY_SIZE]; // 电压频率
    ushort br;  // 00	表示波特率9600(00默认9600，01为4800，02为9600，03为19200，04为38400)
    ushort version;
    ushort reserve;
    uchar ip[10];
};

/**
 * 数据单元：包括当前值，阈值，临界值，告警状态等
 */
struct sUnitCfg
{
    uchar en;
    uchar id;
    int rate;
    float min; // 最小值
    float max; // 最大值

    float crMin; // 最小值
    float crMax; // 最大值
};



/**
 * 数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
struct sObjCfg
{
    sUnitCfg vol; // 电压
    sUnitCfg cur; // 电流
    sUnitCfg output; // 电流
    sUnitCfg opCur[OpSize]; // 输出位电流
    sUnitCfg pow;

    sUnitCfg tem; // 温度
    sUnitCfg hum; // 湿度
    sUnitCfg linevol; // 线电压
    sUnitCfg hz; // 频率
    sUnitCfg totalpow;
    sUnitCfg zerocur; // 零线电流
    sUnitCfg recur; // 剩余电流
};

struct sTypeCfg
{
    uchar si_ac;
    uchar si_led;
    uchar si_horizontal;
    uchar si_lines;
    uchar si_series;
    uchar si_standar;
    uchar si_version;

    uchar si_baud;
    uchar si_iOF;
    uchar si_buzzer;
    uchar si_filter;
    sObjCfg si_cfg;

    uchar ip_bytepassword;
    uchar ip_ac;
    uchar ip_lcd;
    uchar ip_lines;
    uchar ip_language;
    uchar ip_modbus;
    uchar ip_standard;
    uchar ip_version;
    // 1:IP-PDUV1 3:IP-PDUV3 4:IP-PDUV3-C3
    // 5:IP-PDUV3-EATON 6:IP-PDUV6 7:IP-PDUV6-HUAWEI
    // 9:IP_PDUV1_HUADA// IPV1华大 10:IP_PDUV3_BYTE 11:IP_PDUV3_SHATE 12:

    uchar ip_curtype;
    uchar ip_buzzer;
    uchar ip_filter;
    uchar ip_mode;
    uchar ip_num;
    uchar ip_ISD;
    uchar ip_iOF;
    uchar ip_shunt;
    uchar ip_residual;
    uchar ip_lightning;

    uchar security;
    uchar log_en;
    sObjCfg ip_cfg;
    QString ip_inFirst;
    QString ip_inSecond;
    QString ip_outFirst;
    QString ip_outSecond;

    //四个温度
    double temMax[4];
    double temMin[4];
    uchar tem_buzzer;
    uchar tem_filter;

};

struct sDevType
{
    uint devId;
    uchar devType; // 0 SI-PDU  1 IP-PDU
    uchar screen;//0 液晶 1 断码屏 2 数码管 3 V1和断码屏
    uchar lines;
    uchar ac;
    uchar version;
    // IP V1 V3
    // 1:IP-PDUV1 3:IP-PDUV3 4:IP-PDUV3-C3 5:IP-PDUV3-EATON 6:IP-PDUV6 7:IP-PDUV6-HUAWEI
    // 8:IP-PDUV1-YIXIN 9:IP_PDUV1_HUADA 10:IP_PDUV3_BYTE 11:IP_PDUV3_SHATE
    QString dev_type;
    QString sn;
};



/**
 * 设备数据结构体：
 */
struct sDevData
{
    sDevType dt; //设备类型
    sTypeCfg cfg; // 配置数据
    sObjData data;
};


enum {
    Test_Fun, // 功能
    Test_Start, // 开始
    Test_Seting,
    Test_Collect,
    Test_Ading,
    Test_vert, // 验证
    Test_Over, // 终止
    Test_End, // 完成

    Test_Info=0,
    Test_Pass=1,
    Test_Fail=2,
    Test_Standard=0,
    Test_AI=1,
    Test_Onebyone=2,
};

struct sProgress
{
    sProgress() {step=0;}

    uchar step; // 步骤
    uchar recordstep; // 记录启动步骤
    QString time;
    QList<bool> pass;
    QStringList status;

    uchar result;    // 最终结果
    QTime startTime;
    QString softwareType;
    QString companyName;
    QString protocolVersion;

    QString productType;
    QString productSN;
    QString macAddress;
    QString gnd;
    QString ir;
    QString dcw;
    QString acw;
    QString pn;
    // uchar softwareVersion;
    QString softwareVersion;
    QString clientName;
    QString testTime;
    QString testStartTime;
    QString testEndTime;
    QStringList no, itemName;
    QList<bool> uploadPass;
    uchar uploadPassResult;
    QString PCB_Code;//pcb码
};




/**
 * 统计数据结构体
 */
typedef struct _sTgObjData {
    int vol; // 电压
    int cur;  // 电流
    int pow; // 功率

    int ele; // 电能
    int pf; // 功率因素
    int apPow; // 视在功率
    int tem;
}sTgObjData;

typedef struct _sLineTgObjData {
    ushort vol[3]; // 电压
    ushort cur[3];  // 电流

    uint pow[3]; // 功率
    uint ele[3]; // 电能
    ushort pf[3]; // 功率因数
    uint apPow[3]; // 视在功率
    uint reactivePower[3]; // 无功功率
}sLineTgObjData;

/**
 * 数据单元：包括当前值，阈值，临界值，告警状态等
 */
typedef struct _sDataValue {
    ushort value[LINE_NUM_MAX]; // 值
    ushort valued[LINE_NUM_MAX]; // 值

    ushort min[LINE_NUM_MAX]; // 最小值
    ushort max[LINE_NUM_MAX]; // 最大值
    uchar alarm[LINE_NUM_MAX]; // 告警值 0表示未告警  1表示已告警 2表示已记录
    uchar upalarm[LINE_NUM_MAX]; // 告警值 0表示未告警  1表示已告警 2表示已记录
    uchar status[LINE_NUM_MAX]; // 值
    uchar crAlarm[LINE_NUM_MAX]; // 临界告警值

    ushort crMin[LINE_NUM_MAX]; // 临界最小值
    ushort crMax[LINE_NUM_MAX]; // 临界最大值

}sDataValue;

struct sRtuUshortUnit {
    ushort svalue;
    ushort smin;
    ushort smax;//剩余电流时，存储报警界限值

    ushort scrMin; // 临界最小值
    ushort scrMax; // 临界最大值
    uchar supalarm;
    uchar salarm;
    uchar scrAlarm; // 临界告警值
};
struct sRtuUnit {
    uint svalue;
    uint smin;
    uint smax;//剩余电流时，存储报警界限值

    uint scrMin; // 临界最小值
    uint scrMax; // 临界最大值
    uchar supalarm;
    uchar salarm;
    uchar scrAlarm; // 临界告警值
};
struct sRtuULLintUnit {
    unsigned long long ivalue;
    unsigned long long imin;
    unsigned long long imax;

    unsigned long long icrMin; // 临界最小值
    unsigned long long icrMax; // 临界最大值

    uchar iupalarm;
    uchar ialarm;
    uchar icrAlarm; // 临界告警值
};

/**
 * 数据单元：包括当前值，阈值，临界值，告警状态等
 */
typedef struct _sDataPowUnit {
    uint value[LINE_NUM_MAX]; // 值
    uint valued[LINE_NUM_MAX]; // 值

    uint min[LINE_NUM_MAX]; // 最小值
    uint max[LINE_NUM_MAX]; // 最大值
    uchar upalarm[LINE_NUM_MAX]; // 告警值 0表示未告警  1表示已告警 2表示已记录
    uchar alarm[LINE_NUM_MAX]; // 告警值 0表示未告警  1表示已告警 2表示已记录
    uchar status[LINE_NUM_MAX]; // 值
    uchar crAlarm[LINE_NUM_MAX]; // 临界告警值

    uint crMin[LINE_NUM_MAX]; // 临界最小值
    uint crMax[LINE_NUM_MAX]; // 临界最大值
}sDataPowUnit;

/**
 * 插接位数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
typedef struct _sObjectData {
    int lineNum; //相数
    sDataValue vol; // 电压
    // sDataValue cur;  // 电流
    sDataPowUnit cur;
    sDataValue lineVol;  // 线电压

    sDataPowUnit pow; // 功率
    uint ele[LINE_NUM_MAX]; // 电能
    ushort pf[LINE_NUM_MAX]; // 功率因数
    uchar sw[LINE_NUM_MAX]; // 开关状态
    uint apPow[LINE_NUM_MAX]; // 视在功率
    ushort ratedCur[LINE_NUM_MAX]; // 额定电流
    uint reactivePower[LINE_NUM_MAX]; // 无功功率

    ushort pl[3]; // 负载百分比
    ushort curThd[3]; // 电流谐波含量
    ushort volThd[3]; // 电压谐波含量
    sDataPowUnit totalPow;
}sObjectData;


/**
 * 环境数据结构体
 */
typedef struct _sEnvData {
    sDataValue tem; // 温度
}sEnvData;


/**
 * 插接箱数据结构体：包括最多三个插接位，插接箱名称
 */
typedef struct _sBoxData {
    char offLine; // 离线标识
    uchar loopNum; // 回路数量
    uchar version;
    char protocol;// 协议区分标志位
    uchar curSpecification;// 表示电流规格
    uchar workMode;// 0表示始端箱和插接箱主从模式 1表示始端箱Modbus模式
    uchar baudRate;// 表示波特率
    uchar iOF;// 表示iOF触点
    uchar isd;// 表示isd报警触点
    uchar alarmTime;// 表示告警滤波2-5
    uchar shuntRelease;// 表示分励脱扣
    uchar proNum;//项目编号 0：标准 1：定制
    uchar buzzerStatus;//蜂鸣器
    uchar reState;
    uchar boxType;//插接箱 ： 0  温控 ：1
    unsigned long long totalApPow;//   总视在功率

    sObjectData data; // 回路数据
    //char loopName[LINE_NUM_MAX][NAME_LEN]; // 回路名称
    sEnvData env; // 环境状态

    sTgObjData tgBox; // 插接箱统计信息
    sLineTgObjData lineTgBox;
    char boxAlarm, boxVolAlarm, boxCurAlarm, boxEnvAlarm , boxPowerAlarm , boxOffLineAlarm; // 插接箱告警
    char boxStatus; // 插接箱状态
    char boxSpec; //  0 表示 SI  1 表示 IP
    //char boxName[NAME_LEN]; // 插接箱名称

    int ratedCur; // 额定电流
    sRtuUshortUnit rate;//新频率
    sRtuUshortUnit reCur;//剩余电流
    sRtuUnit zeroLineCur;//零线电流
    sRtuULLintUnit totalPow; //总有功功率
    char dc; // 交直流标志位
    uchar lpsState; // 防雷状态 1：工作正常   2：损坏
    uchar lightning; // 防雷是否开启
    uchar lpsAlarm;//防雷开关告警值 0表示未告警 1表示已告警 2表示已记录
    uchar HzAlarm;//频率告警值 0表示未告警 1表示已告警 2表示已记录

    uint zeroCur;//零线电流
    uint volUnbalance;//电压三相不平衡
    uint curUnbalance;//电流三相不平衡
    uint totalCur;//总电流
    uint online1;
    uint online2;

    ushort rtuLen;
    //uchar rtuArray[SRC_DATA_LEN_MAX];
    QString mac;
}sBoxData;


typedef struct _sThdData {
    ushort curThd[3][35]; // 电流谐波含量
    ushort volThd[3][35]; // 电压谐波含量
}sThdData;


/**
 * 母线数据结构体：包括插接箱数据，电压频率 母线名称
 */
typedef struct _sBusData{
    int boxNum; // 插接箱数量
    sBoxData   box[BOX_NUM+1];  // 最多20个插接箱
    //char busName[NAME_LEN]; // 母线名称
    sThdData thdData;
}sBusData;

/**
 * 数据包
 */
class sDataPacket
{
    sDataPacket();
public:
    static sDataPacket *bulid();

    void init();
    sDevData *getDev() {return dev;}
    sProgress *getPro() {return pro;}
    sBusData *share_mem_get();
    void share_mem_init();
    void share_mem_free();
    void share_mem_del();
private:
    sDevData *dev;
    sProgress *pro;
};
extern sBusData *get_share_mem();
#endif // DATAPACKET_H
