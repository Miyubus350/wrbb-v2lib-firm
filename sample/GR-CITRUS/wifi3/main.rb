#!mruby
digitalWrite(5,1)   # LOW:Disable
pinMode(5,1)

Usb = Serial.new(0,115200)

if( System.useWiFi() == 0)then
    Usb.println "WiFi Card can't use."
   System.exit() 
end
Usb.println "WiFi Ready"

Usb.println WiFi.version

Usb.println WiFi.disconnect

Usb.println WiFi.config
Usb.println WiFi.connect("TAROSAY","37000")

Usb.println WiFi.config

Usb.println WiFi.at "CWMODE=1"
Usb.println WiFi.at "CIPMUX=1"
Usb.println 'CIPSTART=4,"TCP","www.yahoo.co.jp",80'


Usb.println "WiFi disconnect"
Usb.println WiFi.disconnect

