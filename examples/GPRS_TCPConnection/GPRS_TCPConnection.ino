
#include <GPRS_Shield_Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Suli.h>

#define PIN_TX    7
#define PIN_RX    8
#define BAUDRATE  19200

char http_cmd[] = "GET /media/uploads/mbed_official/hello.txt HTTP/1.0\r\n\r\n";
char buffer[512];
GPRS gprs(PIN_TX, PIN_RX, BAUDRATE,"cmnet");
void setup(){
  Serial.begin(9600);
  // use DHCP
  gprs.init();
  // attempt DHCP
  while(false == gprs.join()) {
      Serial.println("gprs join network error");
      delay(2000);
  }

  // successful DHCP
  Serial.print("IP Address is ");
  Serial.println(gprs.getIPAddress());

  if(false == gprs.connect(TCP,"mbed.org", 80)) {
      Serial.println("connect error");
  }else{
      Serial.println("connect mbed.org success");
  }

  Serial.println("waiting to fetch...");
  gprs.send(http_cmd, sizeof(http_cmd)-1);
  while (true) {
      int ret = gprs.recv(buffer, sizeof(buffer)-1);
      if (ret <= 0){
          Serial.println("fetch over...");
          break; 
      }
      buffer[ret] = '\0';
      Serial.print("Recv: ");
      Serial.print(ret);
      Serial.print(" bytes: ");
      Serial.println(buffer);
  }
  gprs.close();
  gprs.disconnect();
}

void loop(){

}