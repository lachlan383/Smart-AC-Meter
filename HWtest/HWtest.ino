#define RXD2 16
#define TXD2 17
char msg[] = {0xA5, 0x08, 0x41, 0x00, 0x02, 0x4E, 0x20};
char chksum;
char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void serialHexByte(char b){
  Serial.write(hex[b>>4]);
  Serial.write(hex[b&0x0F]);
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
  Serial.println("Startup...");
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
}

void loop() {

  chksum = 0;
  for (int i = 0; i < sizeof(msg); i++){
    Serial2.print((char)msg[i]);
    chksum += (char)msg[i];
  }
  Serial2.print((char)chksum);

  if (!Serial2.available()){
    delay(10);
  }

  Serial.print("\nuptime: ");
  Serial.println(millis());

  while (Serial2.available()){
    uint32_t tmp;
    if (Serial2.read() == 0x06){
      
      Serial2.read(); // num bytes
      
      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      Serial.write("System Status: ");
      serialHexShort(tmp);
      Serial.write('\n');
      
      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      Serial.write("System Version: ");
      serialHexShort(tmp);
      Serial.write('\n');

      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      Serial.write("Vrms: ");
      serialHexShort(tmp);
      Serial.write('\n');

      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      Serial.write("Line freq: ");
      serialHexShort(tmp);
      Serial.write('\n');

      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      Serial.write("ADC V: ");
      serialHexShort(tmp);
      Serial.write('\n');

      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      Serial.write("Power Factor: ");
      serialHexShort(tmp);
      Serial.write('\n');

      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      tmp += ( Serial2.read() )<<16;
      tmp += ( Serial2.read() )<<24;
      Serial.write("Irms: ");
      serialHexInt(tmp);
      Serial.write('\n');

      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      tmp += ( Serial2.read() )<<16;
      tmp += ( Serial2.read() )<<24;
      Serial.write("Active Power: ");
      serialHexInt(tmp);
      Serial.write('\n');

      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      tmp += ( Serial2.read() )<<16;
      tmp += ( Serial2.read() )<<24;
      Serial.write("Reactive Power: ");
      serialHexInt(tmp);
      Serial.write('\n');

      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      tmp += ( Serial2.read() )<<16;
      tmp += ( Serial2.read() )<<24;
      Serial.write("Apparent Power: ");
      serialHexInt(tmp);
      Serial.write('\n');

      tmp = Serial2.read();
      tmp += ( Serial2.read() )<<8;
      tmp += ( Serial2.read() )<<16;
      tmp += ( Serial2.read() )<<24;
      Serial.write("Energy Accum: ");
      serialHexInt(tmp);
      Serial.write('\n');

      Serial2.read(); // chksum
    }
  }

  delay(3000);
}
