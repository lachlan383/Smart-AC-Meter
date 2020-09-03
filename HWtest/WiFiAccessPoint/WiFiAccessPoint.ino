/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on
  it.

  Steps:
  1. Connect to the access point "ESP32_Power"
  2. Point your web browser to http://192.168.4.1/
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define RXD2 16
#define TXD2 17
char msg[] = {0xA5, 0x08, 0x41, 0x00, 0x02, 0x4E, 0x20};
char chksum;
char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C',
  'D', 'E', 'F'};

// Set these to your desired credentials.
const char *ssid = "ESP32_Power";
const char *password = "lachie383";

WiFiServer server(80);
WiFiClient client;

void serialHexByte(char b){
  client.print(hex[b>>4]);
  client.print(hex[b&0x0F]);
}

void serialHexShort(uint16_t b){
  serialHexByte(b>>8);
  serialHexByte(b&0xFF);
}

void serialHexInt(uint32_t b){
  serialHexShort(b>>16);
  serialHexShort(b&0xFFFF);
}

void setup() {

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial.println();
  Serial.println("Configuring access point...");

  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started");
}

void loop() {
  client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            chksum = 0;
            for (int i = 0; i < sizeof(msg); i++){
              Serial2.print((char)msg[i]);
              chksum += (char)msg[i];
            }
            Serial2.print((char)chksum);
        
            if (!Serial2.available()){
              delay(10);
            }

            client.print("\nuptime: ");
            client.print(millis());
            client.print("<br>");
        
            uint32_t tmp;
            if (Serial2.read() == 0x06){
              
              Serial2.read(); // num bytes
              
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              client.print("System Status: ");
              serialHexShort(tmp);
              client.print("<br>");
              
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              client.print("System Version: ");
              serialHexShort(tmp);
              client.print("<br>");
        
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              client.print("Vrms: ");
              serialHexShort(tmp);
              client.print("<br>");
        
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              client.print("Line freq: ");
              serialHexShort(tmp);
              client.print("<br>");
        
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              client.print("ADC V: ");
              serialHexShort(tmp);
              client.print("<br>");
        
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              client.print("Power Factor: ");
              serialHexShort(tmp);
              client.print("<br>");
        
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              tmp += ( Serial2.read() )<<16;
              tmp += ( Serial2.read() )<<24;
              client.print("Irms: ");
              serialHexInt(tmp);
              client.print("<br>");
        
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              tmp += ( Serial2.read() )<<16;
              tmp += ( Serial2.read() )<<24;
              client.print("Active Power: ");
              serialHexInt(tmp);
              client.print("<br>");
        
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              tmp += ( Serial2.read() )<<16;
              tmp += ( Serial2.read() )<<24;
              client.print("Reactive Power: ");
              serialHexInt(tmp);
              client.print("<br>");
        
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              tmp += ( Serial2.read() )<<16;
              tmp += ( Serial2.read() )<<24;
              client.print("Apparent Power: ");
              serialHexInt(tmp);
              client.print("<br>");
        
              tmp = Serial2.read();
              tmp += ( Serial2.read() )<<8;
              tmp += ( Serial2.read() )<<16;
              tmp += ( Serial2.read() )<<24;
              client.print("Energy Accum: ");
              serialHexInt(tmp);
              client.print("<br>");
        
              Serial2.read(); // chksum
            }

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
