import network
import socket
import time
import machine

# Configuration
SSID = ''
WIFI_PASSWORD = ''
WEB_SERVER_PORT = 8080

# Constants
sensor_temp = machine.ADC(4)

# Connect to wifi
def connect_wlan():
    wlan = network.WLAN(network.STA_IF)

    wlan.active(True)
    wlan.connect(SSID, WIFI_PASSWORD)

    counter = 10
    while counter > 0:
        if wlan.status() < 0 or wlan.status() >= 3:
            break
        
        counter -= 1
        print('Waiting for WiFi connection...')
        time.sleep(1)
        
    if wlan.status() == 3:
        print('Connected to WiFi!')
        status = wlan.ifconfig()
        print('Pico IP: {}'.format(status[0]))
    else:
        raise RuntimeError('Failed to connect to WiFi network')

def get_temperature():
    reading = sensor_temp.read_u16() * (3.3 / (65535))
    temperature = 27 - (reading - 0.706)/0.001721
    return temperature

def start_server():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(('0.0.0.0', WEB_SERVER_PORT))
    s.listen()
    
    while True:
        conn, addr = s.accept()
        conn.recv(1024) # Discard request content
        
        conn.send('HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n')
        conn.send('# HELP pico_temperature temperature from the onboard sensor\n')
        conn.send('# TYPE pico_temperature gauge\n')
        conn.send('pico_temperature {}'.format(get_temperature())) 
        conn.close()

connect_wlan()
start_server()
