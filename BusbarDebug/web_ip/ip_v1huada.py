from ctrlset_ip.ip_web import  *

class IpV1HUADA(IpWeb):

    def start_fun(self):
        self.login()
        self.setCorrect()
        self.setEle()
        self.setThreshold()
        #self.resetFactory()
        self.driver.quit()

    def setCorrect(self):
        cfg = self.cfgs
        security = int(self.cfgs['security'])
        ip =  self.ip_prefix + cfg['ip'] + '/correct.html'
        self.driver.get(ip); time.sleep(1.2)
        self.driver.switch_to.default_content()
        self.setItById("language", int(cfg['ip_language']), '设备语言')
        self.setItById("modbus", cfg['ip_modbus'], '设备模式')
        self.setItById("lcdled", cfg['ip_lines'], '设备相数')
        
        self.setItById("LCDswtich", 1, '新旧屏')
        self.setItById("Code_flag", security, '液晶屏/段码屏')
        self.setItById("CodePrecision", int(self.cfgs['log_en']), '段码屏精度')
        self.setItById("ACDC", cfg['ip_ac'], '交直流')
        self.setMacAddr()
        self.alertClick("Button3")
        self.sendtoMainapp("设备后台网页配置成功", 1)
        self.driver.back(); time.sleep(1)
    








