from ctrlset_ip.ip_web import  *

class IpV3BYTE(IpWeb):

    def start_fun(self):
        #self.ip_prefix = 'https://'
        self.login()
        self.setCorrect()
        #self.login()
        self.setEle()
        self.setIpv3ByteThreshold()
        self.clearLogs()
        #self.resetFactory()
        self.driver.quit()

    def setTime(self):
        self.divClick(8)
        self.execJsAlert("check_time()")
        self.sendtoMainapp("设置设备时间", 1)

    def clearLogs(self):
        en = self.cfgs['log_en']
        if(int(en) < 1):
            return
        self.setTime()
        self.divClick(9)
        jsSheet = "xmlset = createXmlRequest();xmlset.onreadystatechange = setdata;ajaxgets(xmlset, \"/setlclear?a=\" + {0} + \"&\");"
        for num in range(0, 2):
            self.setSelect("loglist", num)
            self.execJs(jsSheet.format(num))
            time.sleep(1)
        self.sendtoMainapp("设备日志清除成功", 1)

    def setCorrect(self):
        cfg = self.cfgs
        security = int(self.cfgs['security'])
        ip = self.ip_prefix + cfg['ip'] + '/correct.html'
        self.driver.get(ip); time.sleep(1.2)
        if(security): time.sleep(1.2)
        self.driver.switch_to.default_content()
        self.setItById("language", cfg['ip_language'], '设备语言')
        self.setItById("modbus", cfg['ip_modbus'], '设备模式')
        self.setItById("lcdled", cfg['ip_lines'], '设备相数')
        self.setItById("ACDC", cfg['ip_ac'], '交直流')
        self.setItById("standard", cfg['ip_standard'], '标准中性')
        self.setItById("LCDswitch", cfg['lcd_switch'], '新旧屏')
        self.setItById("Log_flag", cfg['log_en'], '日志功能')
        index = 0
        if(int(cfg['cur_max']) == 63):
            index = 1
        self.setItById("choiceA", index, '32A/63A选择')
        self.setMacAddr()
        self.alertClick("Button3")
        self.sendtoMainapp("设备后台网页配置成功", 1)
        self.driver.back(); time.sleep(1)
        if(security):time.sleep(1.3)

    





