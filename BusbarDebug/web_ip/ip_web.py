from selenium import webdriver
from selenium.webdriver.support.select import Select
from selenium.common.exceptions import NoSuchElementException
import configparser
import socket
import time
import os


class IpWeb:

    def __init__(self):
        self.ip_prefix = "http://"
        self.initCfg()
        self.initDriver()
        self.initNetWork()

    def initDriver(self):
        try:
            self.driver = webdriver.Firefox(executable_path="geckodriver.exe")
        except ValueError:
            self.driver = webdriver.Chrome(executable_path="chromedriver.exe")

    def initNetWork(self):
        hostname = socket.gethostname()  # 获取计算机名称
        self.dest_ip = socket.gethostbyname(hostname)  # 获取本机IP
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        if "192.168.1." in self.dest_ip:
            return True
        else:
            self.dest_ip = '127.0.0.1'
            # self.sendtoMainapp("Mac地址错误：" + mac, 0)

    def udpSendTo(self, message):
        self.sock.sendto(message.encode('utf-8-sig'), (self.dest_ip, 10086))

    def sendtoMainapp(self, parameter, res):
        message = parameter + ";" + str(res)
        self.udpSendTo(message)

    def setMacAddr(self):
        cfg = self.cfgs
        it = self.driver.find_element_by_id('mac1')
        mac = it.get_attribute('value')
        if "2C:26:5F:" in mac:
            self.udpSendTo("MAC-1")
        else:
            self.setItById("mac1", cfg['mac'], 'Mac地址')

    @staticmethod
    def getCfg():
        cf = configparser.ConfigParser()
        fn = os.path.expanduser('~') + "/.PDU-Settings/cfg.ini"
        cf.read(fn, 'utf-8-sig')  # 读取配置文件，如果写文件的绝对路径，就可以不用os模块
        # cf.read("cfg.ini")
        return cf

    def initCfg(self):
        items = IpWeb.getCfg().items("ipCfg")  # 获取section名为Mysql-Database所对应的全部键值对
        self.cfgs = {'ip_version': 1, 'user': 'admin', 'pwd': 'admin',
                     'ip': '192.168.1.163', 'debug_web': 'correct.html', 'security':0,
                     'ip_lines': 1, 'ip_modbus': 1, 'ip_language': 1, 'lcd_switch': 1,
                     'mac': '', 'ip_ac': 1, 'ip_lcd': 0, 'log_en': 1, 'ip_standard': 0, 'ip_inFirst': 'IEC309', 'ip_inSecond': '18A', 'ip_outFirst': '20xC13', 'ip_outSecond': '4xC19'}
        self.cfgs['mac'] = IpWeb.getCfg().get("Mac", "mac")
        for it in items:
            self.cfgs[it[0]] = it[1]
        if int(self.cfgs['ip_lines']) == 0:
            self.cfgs['ip_lines'] = 1
            self.cfgs['ip_ac'] = 0

    def createAccount(self):
        self.ip_prefix = 'https://'
        ip = self.ip_prefix + self.cfgs['ip'] + '/index.html'
        user = self.cfgs['user'] = 'abcd123'
        pwd = self.cfgs['pwd'] = 'abcd123'
        self.driver.get(ip); time.sleep(3)
        try:
            self.setItById('old_pwd' , user ,'创建账号')
            self.setItById('sign_pwd' , pwd,'创建密码')
            self.setItById('sign_repwd' , pwd,'确认密码')
            self.execJs('changePwd()'); time.sleep(1.2)
            self.sendtoMainapp("创建测试账号成功", 1)
        except:
            self.sendtoMainapp("创建测试账号失败", 1)
        finally:
            self.driver.refresh(); time.sleep(1)
            self.setItById("name", user, '账号')
            self.setItById("psd", pwd, '密码')
            self.execJs("login()");time.sleep(2)

    def inputAccount(self):
        ip = self.ip_prefix + self.cfgs['ip'] + '/'
        user = self.cfgs['user'] = 'admin'
        pwd = self.cfgs['pwd'] = 'admin'
        if(int(self.cfgs['ip_version'])==5):
            user = self.cfgs['user'] = 'eaton'
            pwd = self.cfgs['pwd'] = 'eaton'
        elif(int(self.cfgs['ip_version'])==7):
            user = self.cfgs['user'] = 'huawei'
            pwd = self.cfgs['pwd'] = 'huawei'
        self.driver.get(ip); time.sleep(2.2)
        try:
            self.setItById("name", user, '账号')
            self.setItById("psd", pwd, '密码')
            self.execJs("login()")
            self.sendtoMainapp("网页登陆成功", 1)
        except:
            self.sendtoMainapp("网页登陆失败", 0)
        finally:
            time.sleep(1.2)

    def login(self):
        security = int(self.cfgs['security'])
        v = int(self.cfgs['ip_version'])
        
        if(security and v != 9):
            self.createAccount()
        else:
            self.inputAccount()

    def setCur(self, lines, min, max):
        p = '电流阈值'
        size = lines
        if (size == 2):
            lines = 3
        for num in range(1, lines + 1):
            if (size == 2 and num == 2):
                max = int((int(max) + 1) // 2)
            self.setItById("min" + str(num), int(min), p)
            self.setItById("max" + str(num), int(max), p)
            self.execJs("setlimit({0})".format(num))
            
    def setIpv3ByteCur(self, lines, min, max):
        p = '电流阈值'
        size = lines
        if (size == 2):
            lines = 3
        for num in range(1, lines + 1):
            if (size == 2 and num == 2):
                max = int((int(max) + 1) // 2)
            self.setItById("min" + str(num), int(min), p)
            self.setItById("Lmax" + str(num), round(int(max)*0.8,1), p)
            self.setItById("max" + str(num), int(max), p)
            self.execJs("setlimit({0})".format(num))
            
    def setIpv3ByteThreshold(self):
        curMin = self.cfgs['cur_min']
        curMax = self.cfgs['cur_max']
        volMin = self.cfgs['vol_min']
        volMax = self.cfgs['vol_max']
        line = int(self.cfgs['ip_lines'])
        self.divClick(2)
        self.setIpv3ByteCur(line, curMin, curMax)
        self.setVol(line, volMin, volMax)
        self.setEnv()
        self.sendtoMainapp("设备报警阈值设置成功", 1)
            
    def setIpv6Cur(self, lines, min, max):
        p = '电流阈值'
        lines = 3
        for num in range(1, lines + 1):
            self.setItById("min" + str(num), int(min), p)
            self.setItById("lower_L" + str(num), int(min), p)
            self.setItById("Upper_L" + str(num), int(max), p)
            self.setItById("max" + str(num), int(max), p)
            self.execJs("setlimit({0})".format(num))

    def setVol(self, lines, min, max):
        p = '电压阈值'
        size = lines
        if (size == 2):
            lines = 3
        for num in range(4, lines + 4):
            self.setItById("min" + str(num), min, p)
            self.setItById("max" + str(num), max, p)
            self.execJs("setlimit({0})".format(num))

    def setEnv(self):
        temMin = self.cfgs['tem_min']
        temMax = self.cfgs['tem_max']
        humMin = self.cfgs['hum_min']
        humMax = self.cfgs['hum_max']

        p = '温度阈值'
        self.setItById("min7", temMin, p)
        self.setItById("max7", temMax, p)
        self.execJs("setlimit(7)")

        p = '湿度阈值'
        self.setItById("min8", humMin, p)
        self.setItById("max8", humMax, p)
        self.execJs("setlimit(8)")

    def setSelectLcd(self, id, v):
        try:
            time.sleep(0.15)
            it = self.driver.find_element_by_id(id)
        except NoSuchElementException:
            msg = '网页上找不到{0}'.format(id)
        else:
            try:
                if it.is_displayed():
                    Select(it).select_by_index(v); time.sleep(1)
                    self.execJs("setdevice()"); time.sleep(1)
                    self.driver.switch_to.alert.accept()
            except:
                pass

    def setLcdDir(self):
        dir = self.cfgs['ip_lcd']
        self.setSelectLcd("dir", dir)
        self.setSelectLcd("direction", dir)

    def setEle(self):
        self.divClick(3)
        self.setLcdDir()
        jsSheet = " claerset = createXmlRequest();claerset.onreadystatechange = clearrec;ajaxget(claerset, \"/energyzero?a=\" + {0}+\"&\");"
        for num in range(0, 4):
            self.execJs(jsSheet.format(num))
        self.sendtoMainapp("设备电能清除成功", 1)

    def setThreshold(self):
        curMin = self.cfgs['cur_min']
        curMax = self.cfgs['cur_max']
        volMin = self.cfgs['vol_min']
        volMax = self.cfgs['vol_max']
        line = int(self.cfgs['ip_lines'])
        self.divClick(2)
        self.setCur(line, curMin, curMax)
        self.setVol(line, volMin, volMax)
        self.setEnv()
        self.sendtoMainapp("设备报警阈值设置成功", 1)
    
    def setIpv6Threshold(self):
        curMin = self.cfgs['cur_min']
        curMax = self.cfgs['cur_max']
        volMin = self.cfgs['vol_min']
        volMax = self.cfgs['vol_max']
        line = 3
        self.setIpv6Cur(line, curMin, curMax)
        self.setVol(line, volMin, volMax)
        self.setEnv()
        self.sendtoMainapp("设备后台报警阈值设置成功", 1)

    def setSelect(self, id, v):
        time.sleep(0.35)
        it = self.driver.find_element_by_id(id)
        if it.is_displayed():
            Select(it).select_by_index(v)
            time.sleep(0.8)

    def setItById(self, id, v, parameter):
        try:
            time.sleep(0.1)
            it = self.driver.find_element_by_id(id)
        except NoSuchElementException:
            msg = '网页上找不到{0}'.format(id)
        else:
            if it.is_displayed():
                it.clear();time.sleep(0.1)
                it.send_keys(str(v))
                msg = '设置{0} {1}：{2}'.format(parameter, id, v)
                self.sendtoMainapp(msg, 1)

    def btnClick(self, id):
        try:
            self.driver.find_element_by_id(id).click()
            time.sleep(0.8)
        except:
            msg = '网页上找不到{0}'.format(id)
        

    def alertClick(self, id):
        self.btnClick(id); time.sleep(0.5)
        self.driver.switch_to.alert.accept()
        time.sleep(0.8)

    def divClick(self, id):
        security = int(self.cfgs['security'])
        self.driver.switch_to.default_content()
        self.execJs("clk({0})".format(id))
        self.driver.switch_to.frame('ifrm')
        if(security):time.sleep(1.2)
        time.sleep(0.8)

    def execJs(self, js):
        try:
            security = int(self.cfgs['security'])
            self.driver.execute_script(js);time.sleep(1)
            if(security):time.sleep(2.5)
        except:
            msg = '网页上找不到此方法{0}'.format(js)

    def execJsAlert(self, js):
        self.execJs(js); time.sleep(0.6)
        self.driver.switch_to.alert.accept()
        time.sleep(0.8)

    def resetFactory(self):
        v = IpWeb.getCfg().get("ipCfg", "ip_version")
        if (1 == int(v)):
            self.divClick(8)
        else:
            self.divClick(10)
        self.setSelect("order", 1)
        self.execJsAlert("setdevice();")
        self.sendtoMainapp("设备Web出厂设置成功", 1)
        time.sleep(8)
        
    def itemCheck(self, ssid, value, parameter):
        try:
            it = self.findItById(ssid)
            if it != None:
                v = it.get_attribute('value')
                msg = '检测{0}，期望值{1}，实际值{2}'.format(parameter, value, v)
                if (str(value) != str(v)):
                    self.sendtoMainapp('错误 '+msg, 0)
                else:
                    self.sendtoMainapp(msg, 1)
        except:
            pass
    
    def macAddrCheck(self):
        it = self.findItById('mac1')
        mac = it.get_attribute('value')
        if "2C:26:5F:" in mac:
            self.sendtoMainapp("Mac地址合法："+mac, 1)
        else:
            self.sendtoMainapp("Mac地址错误："+mac, 0)
    
    def findItById(self, id):
        try:
            time.sleep(0.15)
            it = self.driver.find_element_by_id(id)
        except NoSuchElementException:
            msg = '网页上找不到{0}'.format(id)
            #self.sendtoMainapp(msg, 1)
            return None
        else:
            return it
    
    def reStart(self):
        self.divClick(10)
        self.setSelect("order", 0)
        self.execJsAlert("setdevice();")
        self.sendtoMainapp("重启成功", 1)
        time.sleep(10)




