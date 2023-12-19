from ctrlset_ip.ip_web import  *

class IpV6(IpWeb):

    def start_fun(self):
        self.login()
        self.setCorrect()
        self.setEle()
        if(int(self.cfgs['version'])==6):
            self.divClick(2)
            self.checkIpv6Threshold()
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
        self.setItById("language", int(cfg['ip_language'])+1, '设备语言')
        self.setMacAddr()
        self.alertClick("Button3")
        time.sleep(1)
        if(int(self.cfgs['ip_version'])==7):
            self.btnClick("Button2")
            self.driver.switch_to.frame('ifrm')
            self.setIpv6Threshold()
            self.sendtoMainapp("设备后台网页配置成功", 1)
            self.driver.back(); time.sleep(1)
        self.driver.back(); time.sleep(1)
        if(security):time.sleep(1.3)

    





