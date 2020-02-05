import subprocess, socket
import os , sys, signal
from threading import Thread
import platform
dir_path = os.path.dirname(os.path.realpath(__file__))

def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # doesn't even have to be reachable
        s.connect(('10.255.255.255', 1))
        IP = s.getsockname()[0]
    except:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP

clients = {}
ip_of_sender = get_ip() # subprocess.check_output("hostname -I" , shell=True).decode('utf-8').rstrip()
esp_respond_sender_port = 3235

if platform.system() == "Windows":
	firmware = dir_path+'/../.pio/build/esp-wrover-kit/firmware.bin'
else:
	firmware = dir_path+'/../.pioenvs/esp-wrover-kit/firmware.bin'

version_filter = False
if  len(sys.argv) > 1:
	version_filter = str(sys.argv[1])

upgradeCount = 0


from zero import *
import time

class ServiceListener(object):
    def __init__(self):
        self.r = Zeroconf()

    def removeService(self, zeroconf, type, name):
        print()
        print( "Service", name, "removed")

    def addService(self, zeroconf, type, name):
        #print( "  Type is", type)
        info = self.r.getServiceInfo(type, name)
        if info:
            clients[name] = {}
            clients[name]['ip'] = str(socket.inet_ntoa(info.getAddress())).rstrip()
            clients[name]['port'] =  str(info.getPort())
            try:
                clients[name]['version'] = name.split('v')[1][:4]
            except:
                clients[name]['version'] = 0
            clients[name]['upgrade'] = (not version_filter or clients[name]['version'] != version_filter)
            if clients[name]['upgrade']:
                global upgradeCount
                upgradeCount += 1
            print "Device ", len(clients),": ", name.split(type)[0], "UPGRADABLE" if  clients[name]['upgrade'] else "OK"

            #print( "  Weight is %d, Priority is %d" % (info.getWeight(),
            #                                          info.getPriority()))
            #print( "  Server is", info.getServer())
            #prop = info.getProperties()
            #if prop:
            #    print( "  Properties are")
            #    for key, value in prop.items():
            #        print( "    %s: %s" % (key, value))


class Upgrader(Thread):
    def __init__(self, port, info):
        Thread.__init__(self)
        self.port = port
        self.info = info

    def run(self):
        stri = "Uploading to "+str(self.info['ip'])
        print stri
        cmd = 'python ~/Arduino/hardware/espressif/esp32/tools/espota.py -i '+self.info['ip']+' -I '+ip_of_sender+' -p '+self.info['port']+' -P '+str(self.port)+' -f '+firmware
        p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        if (p.wait() == 0):
            stri = "Updated " + str(self.info['ip'])
        else:
            stri = "Failed " + str(self.info['ip'])
        print stri


if __name__ == '__main__':
    while True:
        upgradeCount = 0
        clients = {}
        subprocess.call('clear',shell=True)
        r = Zeroconf()
        type = "_arduino._tcp.local."
        listener = ServiceListener()
        browser = ServiceBrowser(r, type, listener)
        # print("LOCAL ip: "+ str(ip_of_sender) +"\n")
        print "============================================="
        print "============ ESP32 - OTA updater ============"
        print "============================================="
        print ""
        print "Firmware date: ", time.ctime(os.stat(firmware).st_mtime)
        print ""

        try:
            a = raw_input("Detecting devices... Press Enter when ready.\n\n")
        except KeyboardInterrupt:
            r.close()
            browser.cancel();
            listener.r.close();
            print "Goodbye :)"
            os.kill(os.getpid(), signal.SIGKILL)
			exit(1)

        r.close()
        #time.sleep(2)
        y = raw_input("Upload to "+str(upgradeCount)+" Devices ?... [y/n]\n\n")
        if y is not 'y':
            continue

        threadlist = []
        for esp, info in clients.items():
            if info['upgrade']:
                # print "Preparing upload to ", info['ip']
                threadlist.append(Upgrader(esp_respond_sender_port, info))
                esp_respond_sender_port += 1

        for t in threadlist:
            t.start()

        for t in threadlist:
            t.join()
