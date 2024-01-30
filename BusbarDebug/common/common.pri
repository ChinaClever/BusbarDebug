
INCLUDEPATH += $$PWD

include(json/json.pri)
include(excel/excel.pri)
include(dbcom/dbcom.pri)
include(sqlcom/sqlcom.pri)
include(logcom/logcom.pri)
include(cfgcom/cfgcom.pri)
include(msgcom/msgcom.pri)
include(tabcom/tabcom.pri)
include(modbus/modbus.pri)
#include(printer/printer.pri)
include(network/network.pri)
#include(qrencode/qrencode.pri)
include(websocket/websocket.pri)
include(smtpclient/smtpclient.pri)
include(snmpclient/snmpclient.pri)
include(datapacket/datapacket.pri)
include(serialport/serialport.pri)
include(backcolour/backcolour.pri)
include(qtsingleapplication/qtsingleapplication.pri)
include(http/http.pri)

HEADERS += \
    $$PWD/common.h
	
SOURCES += \
    $$PWD/common.cpp

FORMS += \




