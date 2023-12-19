from ctrlset_ip.ip_web import  *

class IpV3EATON(IpWeb):

    def start_fun(self):
        self.login()
        self.setCorrect()
        self.setEle()
        self.setThreshold()
        self.reStart()
        self.driver.refresh()
        self.login()
        self.checkCorrect()
        self.checkThreshold()
        #self.clearLogs()
        #self.resetFactory()
        self.driver.quit()


    def setCorrect(self):
        cfg = self.cfgs
        ip = self.ip_prefix + cfg['ip'] + '/correct.html'
        self.driver.get(ip); time.sleep(1.2)
        self.driver.switch_to.default_content()
        self.setItById("language", cfg['ip_language'], '设备语言')
        self.setItById("modbus", cfg['ip_modbus'], '设备模式')
        self.setItById("lcdled", cfg['ip_lines'], '设备相数')
        self.setItById("LCDswitch", cfg['lcd_switch'], '新旧屏')
        self.setItById("standard", cfg['ip_standard'], '标准中性')
        self.setItById("Log_flag", cfg['log_en'], '日志功能')
        self.setMacAddr()
        self.setItById("IN", cfg['ip_infirst'], '输入ex')
        self.setItById("IN1", cfg['ip_insecond'], '输入ea')
        self.setItById("OUT1", cfg['ip_outfirst'], '输出ex')
        self.setItById("OUT2", cfg['ip_outsecond'], '输出ea')
        time.sleep(2)
        self.alertClick("Button3")
        self.sendtoMainapp("设备后台网页配置成功", 1)
        time.sleep(2)
        self.driver.back(); time.sleep(1)
        
    def checkCorrect(self):
        cfg = self.cfgs
        ip =  self.ip_prefix + cfg['ip'] + '/correct.html'
        self.driver.get(ip); time.sleep(1.1)
        self.driver.switch_to.default_content()
        self.itemCheck("language", int(cfg['ip_language']), '语言选择')
        self.itemCheck("lcdled", cfg['ip_lines'], '相数选择')
        self.itemCheck("modbus", cfg['ip_modbus'], '设备模式')
        self.itemCheck("LCDswitch", cfg['lcd_switch'], '新旧屏')
        self.itemCheck("standard", cfg['ip_standard'], '标准中性')
        self.itemCheck("Log_flag", cfg['log_en'], '日志功能')
        self.macAddrCheck()
        self.itemCheck("IN", cfg['ip_infirst'], '输入ex')
        self.itemCheck("IN1", cfg['ip_insecond'], '输入ea')
        self.itemCheck("OUT1", cfg['ip_outfirst'], '输出ex')
        self.itemCheck("OUT2", cfg['ip_outsecond'], '输出ea')
        self.driver.back(); time.sleep(1)
        
    def checkThreshold(self):
        curMin = self.cfgs['cur_min']
        curMax = self.cfgs['cur_max']
        volMin = self.cfgs['vol_min']
        volMax = self.cfgs['vol_max']
        line = int(self.cfgs['ip_lines'])
        self.divClick(2)
        self.checkCur(line, curMin, curMax)
        self.checkVol(line, volMin, volMax)
        self.checkEnv()
        self.sendtoMainapp("设备报警阈值设置成功", 1)
    
    def checkCur(self , line, curMin, curMax):
        p = '电流阈值'
        size = line
        if (size == 2):
            line = 3
        for num in range(1, line + 1):
            if (size == 2 and num == 2):
                curMax = int((int(curMax) + 1) // 2)
            self.itemCheck("min" + str(num), curMin, p+'最小值')
            self.itemCheck("max" + str(num), str(curMax), p+'最大值')
            
    def checkVol(self, lines, min, max):
        p = '电压阈值'
        size = lines
        if (size == 2):
            lines = 3
        for num in range(4, lines + 4):
            self.itemCheck("min" + str(num), min, p+'最小值')
            self.itemCheck("max" + str(num), max, p+'最大值')
            
    def checkEnv(self):
        temMin = self.cfgs['tem_min']
        temMax = self.cfgs['tem_max']
        humMin = self.cfgs['hum_min']
        humMax = self.cfgs['hum_max']

        p = '温度阈值'
        self.itemCheck("min7", temMin, p+'最小值')
        self.itemCheck("max7", temMax, p+'最大值')

        p = '湿度阈值'
        self.itemCheck("min8", humMin, p+'最小值')
        self.itemCheck("max8", humMax, p+'最大值')

    





