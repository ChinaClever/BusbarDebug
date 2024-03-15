/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "json_pack.h"


Json_Pack::Json_Pack(QObject *parent)
{
    mPro = sDataPacket::bulid()->getPro();
}

Json_Pack *Json_Pack::bulid(QObject *parent)
{
    static Json_Pack* sington = NULL;
    if(sington == NULL) {
        sington = new Json_Pack(parent);
    }
    return sington;
}


void Json_Pack::head(QJsonObject &obj)
{
    QDateTime t = QDateTime::currentDateTime();
    mPro->testEndTime = t.toString("yyyy-MM-dd HH:mm:ss");
    mPro->testTime = QString::number(QDateTime::fromString(mPro->testStartTime,"yyyy-MM-dd HH:mm:ss").secsTo(t));
    obj.insert("softwareType", mPro->softwareType);
    obj.insert("productType", mPro->productType);
    obj.insert("productSN", mPro->productSN);
    obj.insert("macAddress", mPro->macAddress);
    obj.insert("result", mPro->uploadPassResult);
    obj.insert("softwareVersion", mPro->softwareVersion);
    obj.insert("work_order", mPro->clientName);//工单号
    obj.insert("companyName", mPro->companyName);
    obj.insert("protocolVersion", mPro->protocolVersion);
    obj.insert("testStartTime", mPro->testStartTime);
    obj.insert("testEndTime", mPro->testEndTime);
    obj.insert("testTime", mPro->testTime);
    obj.insert("PCB_Code", mPro->PCB_Code);

    pduInfo(obj);
}

void Json_Pack::pduInfo(QJsonObject &obj)
{
    objData(obj);
}

void Json_Pack::part_head(QJsonObject &obj)
{
    obj.insert("serial_id", mPro->productSN);
    obj.insert("work_order", mPro->clientName);


}

int Json_Pack::objData(QJsonObject &obj)
{
    QJsonArray jsonArray;
    int num = mPro->uploadPass.size();
    for(int i=0; i<num; ++i)
    {
        QJsonObject subObj;
        subObj.insert("no",i+1);
        subObj.insert("name", mPro->itemName.at(i));
        subObj.insert("result", mPro->uploadPass.at(i)?1:0);

        jsonArray.append(subObj);
    }
    obj.insert("testStep" ,QJsonValue(jsonArray));

    return num;
}

void Json_Pack::getJson(QJsonObject &json , QByteArray &ba)
{
    head(json);
    QJsonDocument jsonDoc(json);
    ba = jsonDoc.toJson();
}
void Json_Pack::http_post(const QString &method, const QString &ip, int port)
{
    QJsonObject json; head(json);
    AeaQt::HttpClient http;
    http.clearAccessCache();
    http.clearConnectionCache();
    QString url = "http://%1:%2/%3";
    http.post(url.arg(ip).arg(port).arg(method))
        .header("content-type", "application/json")
        .onSuccess([&](QString result) {qDebug()<<"http_post_result   "<< result;mPro->result = Test_Over;emit httpSig("数据发送成功",true);})
        .onFailed([&](QString error) {qDebug()<<"http_post_error   "<< error;mPro->result = Test_Fail;emit httpSig("数据发送失败",false); })
        .onTimeout([&](QNetworkReply *) {qDebug()<<"http_post timeout   ";mPro->result = Test_Fail;emit httpSig("http_post timeout",false); }) // 超时处理
        .timeout(1000) // 1s超时
        .block()
        .body(json)
        .exec();
}
QString Json_Pack::http_get(const QString &method, const QString &ip, int port)
{
    QJsonObject json; part_head(json);
    QString mac;
    AeaQt::HttpClient http;
    http.clearAccessCache();
    http.clearConnectionCache();
    QString url = "http://%1:%2/%3";
    http.get(url.arg(ip).arg(port).arg(method))
        .header("content-type", "application/json")
        .onSuccess([&](QString result) {qDebug()<<"http_get_result   "<< result; mac = result;emit httpSig(result,true);})
        .onFailed([&](QString error) {qDebug()<<"http_get_error   "<< error;emit httpSig(error,false); })
        .onTimeout([&](QNetworkReply *) {qDebug()<<"http_get timeout   ";emit httpSig("http_get timeout",false); }) // 超时处理
        .timeout(1000) // 1s超时
        .block()
        .body(json)
        .exec();
    return mac;
}
//bool Json_Build::saveJson( QJsonObject &json)
//{
//    QJsonDocument jsonDoc(json);
//    QByteArray ba = jsonDoc.toJson();
//    QString path = CfgCom::bulid()->pathOfData("pdu_id.json");
//    QFile file(path);
//    bool ret = false;
//    if(file.exists())//文件存在则不需要再写
//    {
//        ret = true;
//    }
//    else//文件不存在则写入初始数据
//    {
//        ret = file.open(QIODevice::WriteOnly);
//        if(ret) {
//            file.write(ba);
//            file.close();
//        } else {
//            qDebug() << "write json file failed";
//        }
//    }

//    return ret;
//}

