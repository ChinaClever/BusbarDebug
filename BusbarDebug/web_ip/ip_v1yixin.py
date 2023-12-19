from ctrlset_ip.ip_web import  *

class IpV1YIXIN(IpWeb):

    def start_fun(self):
        self.login()
        self.setCorrect()
        self.setEle()
        self.setValue()
        self.driver.quit()

    def setCorrect(self):
        cfg = self.cfgs
        ip =  self.ip_prefix + cfg['ip'] + '/correct.html'
        self.driver.get(ip); time.sleep(1.2)
        self.driver.switch_to.default_content()
        self.setItById("language", int(cfg['ip_language'])+1, '设备语言')
        self.setMacAddr()
        self.alertClick("Button3")
        self.sendtoMainapp("设备后台网页配置成功", 1)
        self.driver.back(); time.sleep(1)
    
    def inputAccount(self):
        ip = self.ip_prefix + self.cfgs['ip'] + '/'
        user = self.cfgs['user'] = 'admin'
        pwd = self.cfgs['pwd'] = 'admin'
        try:
            self.driver.get(ip);
            self.setItById("name", user, '账号')
            self.setItById("psd", pwd, '密码')
            self.execJs("login()")
            self.sendtoMainapp("网页登陆成功", 1)
        except:
            self.sendtoMainapp("网页登陆失败", 0)
        finally:
            time.sleep(1.2)

    def login(self):
        self.inputAccount()
    
    def setValue(self):
        curMin = self.cfgs['cur_min']
        curMax = self.cfgs['cur_max']
        volMin = self.cfgs['vol_min']
        volMax = self.cfgs['vol_max']
        line = 1
        self.divClick(2)
        self.setCur(line, curMin, curMax)
        self.setVol(line, volMin, volMax)
        self.setEnvValue()
        self.setOtherValue()
        self.sendtoMainapp("设备报警阈值设置成功", 1)
        
    def setEnvValue(self):    
        temMin = self.cfgs['tem_min']
        temMax = self.cfgs['tem_max']
        humMin = self.cfgs['hum_min']
        humMax = self.cfgs['hum_max']

        p = '温度1阈值'
        self.setItById("min7", temMin, p)
        self.setItById("max7", temMax, p)
        self.execJs("setlimit(7)")
        p = '湿度1阈值'
        self.setItById("min8", humMin, p)
        self.setItById("max8", humMax, p)
        self.execJs("setlimit(8)")

        p = '温度2阈值'
        self.setItById("min9", temMin, p)
        self.setItById("max9", temMax, p)
        self.execJs("setlimit(9)")
        p = '湿度2阈值'
        self.setItById("min10", humMin, p)
        self.setItById("max10", humMax, p)
        self.execJs("setlimit(10)")
    
    def setOtherValue(self): 
        fanMin = self.cfgs['fantem_min']
        fanMax = self.cfgs['fantem_max']
        pwmMax = self.cfgs['pwm_max']
        
        p = '风扇曲线温度阈值'
        self.setItById("min11", fanMin, p)
        self.setItById("max11", fanMax, p)
        self.execJs("setlimit(11)")
        p = 'PWM上限阈值'
        self.setItById("max12", pwmMax, p)
        self.execJs("setlimit(12)")





