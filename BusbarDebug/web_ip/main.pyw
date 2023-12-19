from ctrlset_ip.ip_v1 import  *
from ctrlset_ip.ip_v3 import  *
from ctrlset_ip.ip_v3c3 import  *
from ctrlset_ip.ip_v3eaton import  *
from ctrlset_ip.ip_v6 import  *
from ctrlset_ip.ip_v1yixin import  *
from ctrlset_ip.ip_v1huada import  *
from ctrlset_ip.ip_v3byte import  *



if __name__ == '__main__':
    v = IpWeb.getCfg().get("ipCfg", "ip_version")

    if(int(v) == 3 ):
        app = IpV3()
        app.start_fun()
    elif(int(v) == 4):
        app = IpV3C3()
        app.start_fun()
    elif(int(v) == 5):
        app = IpV3EATON()
        app.start_fun()
    elif(int(v) == 6 or int(v) == 7):
        app = IpV6()
        app.start_fun()
    elif(int(v) == 8):
        app = IpV1YIXIN()
        app.start_fun()
    elif(int(v) == 9):
        app = IpV1HUADA()
        app.start_fun()
    elif(int(v) == 10):
        app = IpV3BYTE()
        app.start_fun()    
    else:
        app = IpV1()
        app.start_fun()

